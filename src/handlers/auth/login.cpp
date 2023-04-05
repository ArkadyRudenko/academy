#include "login.hpp"
#include "../../auth/user_info_cache.hpp"
#include "../../db/db_requests.hpp"

#include <boost/uuid/random_generator.hpp>
#include "userver/utils/boost_uuid4.hpp"

namespace handlers::auth {

UserLogin::UserLogin(const components::ComponentConfig& config,
                     const components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      pg_cluster_(
          context.FindComponent<userver::components::Postgres>("academy")
              .GetCluster()) {}

formats::json::Value UserLogin::HandleRequestJsonThrow(
    const server::http::HttpRequest&, const formats::json::Value& user_json,
    server::request::RequestContext&) const {
  std::string login;
  try {
    login = user_json["login"].As<std::string>();  // TODO is need check json everywhere?
  } catch (...) {
    throw server::handlers::ClientError(server::handlers::ExternalBody{"Invalid 'login' query argument"});
  }

  const auto result = pg_cluster_->Execute(
      storages::postgres::ClusterHostType::kMaster, db::kGetUserByLogin, login);

  if (result.IsEmpty()) {
    throw server::handlers::ClientError(server::handlers::ExternalBody{"login not found"});
  }

  const auto& user =
      result.AsSingleRow<model::User>(userver::storages::postgres::kRowTag);

  if (user.password == user_json["password"].As<std::string>()) { // TODO generate secret password
    return formats::json::MakeObject("access_token", GenerateAccessToken(user));
  }
}

std::string UserLogin::GenerateAccessToken(const model::User& user) const {
  const auto token = utils::generators::GenerateBoostUuid();

  auto token_string = utils::ToString(token);

  const auto result = pg_cluster_->Execute(
      storages::postgres::ClusterHostType::kMaster, db::kInsertAccessToken,
      token_string, user.user_id, std::vector{"read"}, user.full_name);

  if (!result.IsEmpty()) {
    return token_string;
  }
  return "none";
}

void AppendUserLogin(components::ComponentList& component_list) {
  component_list.Append<UserLogin>();
}
}  // namespace handlers::auth
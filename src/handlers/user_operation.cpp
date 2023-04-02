#include "user_operation.hpp"
#include "../model/user.hpp"

#include <userver/storages/postgres/io/row_types.hpp>

namespace handlers {

const storages::postgres::Query kRegisterUser{
    "INSERT INTO academy.new_users (full_name, tg_id) VALUES($1, $2) "
    "RETURNING new_users",
    storages::postgres::Query::Name{"insert_user"},
};

const storages::postgres::Query kGetUserById{
    "SELECT * FROM academy.new_users WHERE id = $1",
    storages::postgres::Query::Name{"get_user_by_id"},
};

UserOperation::UserOperation(
    const components::ComponentConfig& config,
    const components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      pg_cluster_(component_context
                      .FindComponent<userver::components::Postgres>("academy")
                      .GetCluster()) {}

formats::json::Value UserOperation::HandleRequestJsonThrow(
    const server::http::HttpRequest& request, const formats::json::Value& json,
    server::request::RequestContext&) const {
  switch (request.GetMethod()) {
    case server::http::HttpMethod::kGet:
      return GetUserById(std::atoi(request.GetArg("id").c_str()));
    case server::http::HttpMethod::kPost:
      return Register(json);
    case server::http::HttpMethod::kDelete:
      return {};
    default:
      throw server::handlers::ClientError(server::handlers::ExternalBody{
          fmt::format("Unsupported method {}", request.GetMethod())});
  }
  return formats::json::Value();
}

formats::json::Value UserOperation::Register(
    const formats::json::Value& json) const {
  const auto& full_name = json["full_name"].As<std::string>();
  const auto& tg_id = json["tg_id"].As<std::string>();

  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, kRegisterUser,
      full_name, tg_id);

  const auto& user = result.AsSingleRow<model::User>();

  return model::User::ToJson(user);
}

formats::json::Value UserOperation::GetUserById(
    const std::int32_t user_id) const {
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, kGetUserById,
      user_id);

  if (result.IsEmpty()) {
    return formats::json::MakeObject("Status", "Failed");
  }

  const auto& user = result.AsSingleRow<model::User>(userver::storages::postgres::kRowTag);

  return model::User::ToJson(user);
}

void AppendUser(userver::components::ComponentList& component_list) {
  component_list.Append<UserOperation>();
}

}  // namespace handlers
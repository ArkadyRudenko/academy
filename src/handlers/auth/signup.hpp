#pragma once

#include "userver/utest/using_namespace_userver.hpp"

#include "userver/components/minimal_server_component_list.hpp"

#include "../../db/db_requests.hpp"
#include "../../model/user.hpp"

#include "userver/server/handlers/http_handler_json_base.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/component.hpp"

namespace handlers::auth {

class UserSignUp final : public server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "handler-signup";

  UserSignUp(const components::ComponentConfig& config,
             const components::ComponentContext& context)
      : HttpHandlerJsonBase(config, context),
        pg_cluster_(
            context.FindComponent<userver::components::Postgres>("academy")
                .GetCluster()) {}

  formats::json::Value HandleRequestJsonThrow(
      const server::http::HttpRequest&, const formats::json::Value& user_json,
      server::request::RequestContext&) const override {
    const auto result = pg_cluster_->Execute(
        storages::postgres::ClusterHostType::kMaster, db::kRegisterUser,
        user_json["login"].As<std::string>(),
        user_json["password"].As<std::string>(),
        user_json["full_name"].As<std::string>(),
        user_json["tg_id"].As<std::string>());

    // TODO full_name is primary-key
    const auto& user = result.AsSingleRow<model::User>();

    return model::User::ToJson(user);
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

void AppendUserSignUp(components::ComponentList& component_list) {
  component_list.Append<UserSignUp>();
}

}  // namespace handlers::auth
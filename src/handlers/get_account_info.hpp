#pragma once

#include "../db/db_requests.hpp"
#include "../model/user.hpp"

#include "userver/utest/using_namespace_userver.hpp"

#include "userver/server/handlers/http_handler_json_base.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/component.hpp"

#include "userver/components/minimal_server_component_list.hpp"

namespace handlers {

class GetAccountInfo final : public server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "handler-account";

  GetAccountInfo(const components::ComponentConfig& config,
                 const components::ComponentContext& context)
      : HttpHandlerJsonBase(config, context),
        pg_cluster_(
            context.FindComponent<userver::components::Postgres>("academy")
                .GetCluster()) {}

  formats::json::Value HandleRequestJsonThrow(
      const server::http::HttpRequest&, const formats::json::Value& user_json,
      server::request::RequestContext&) const override {
    const auto result = pg_cluster_->Execute(
        storages::postgres::ClusterHostType::kMaster,
        db::kGetUserById, user_json["user_id"].As<std::int32_t>());

    return model::User::ToJson(result.AsSingleRow<model::User>(storages::postgres::kRowTag));
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

void AppendGetAccountInfo(components::ComponentList& component_list) {
  component_list.Append<GetAccountInfo>();
}

}  // namespace handlers
#pragma once

#include "../../model/user.hpp"

#include "userver/utest/using_namespace_userver.hpp"

#include "userver/components/minimal_server_component_list.hpp"
#include "userver/formats/json.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/component.hpp"

namespace handlers::auth {

class UserLogin final : public server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "handler-login";

  UserLogin(const components::ComponentConfig& config,
            const components::ComponentContext& context);

  formats::json::Value HandleRequestJsonThrow(
      const server::http::HttpRequest&, const formats::json::Value& user_json,
      server::request::RequestContext&) const override;

 private:
  std::string GenerateAccessToken(const model::User& user) const;

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

void AppendUserLogin(components::ComponentList& component_list);

}  // namespace handlers::auth
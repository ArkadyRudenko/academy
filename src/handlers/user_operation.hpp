#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/rcu/rcu.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utils/datetime.hpp>

#include <userver/formats/json.hpp>

#include <userver/utest/using_namespace_userver.hpp>

namespace handlers {

class UserOperation final : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "user";

  using KeyValues = std::unordered_map<std::string, formats::json::Value>;

  // Component is valid after construction and is able to accept requests
  UserOperation(const components::ComponentConfig& config,
                    const components::ComponentContext& context);

  formats::json::Value HandleRequestJsonThrow(
      const server::http::HttpRequest&, const formats::json::Value& json,
      server::request::RequestContext&) const override;

 private:
  formats::json::Value Register(const formats::json::Value& json) const;
  formats::json::Value GetUserById(std::int32_t user_id) const;
 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

void AppendUser(userver::components::ComponentList& component_list);

} // namespace handlers
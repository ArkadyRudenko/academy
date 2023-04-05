#pragma once

#include <userver/utest/using_namespace_userver.hpp>  // only for samples
#include <userver/server/handlers/auth/auth_checker_factory.hpp>

#include "user_info_cache.hpp"

namespace auth {

class CheckerFactory final
    : public server::handlers::auth::AuthCheckerFactoryBase {
 public:
  server::handlers::auth::AuthCheckerBasePtr operator()(
      const ::components::ComponentContext& context,
      const server::handlers::auth::HandlerAuthConfig& auth_config,
      const server::handlers::auth::AuthCheckerSettings&) const override;
};

class AuthCheckerBearer final : public server::handlers::auth::AuthCheckerBase {
 public:
  using AuthCheckResult = server::handlers::auth::AuthCheckResult;

  AuthCheckerBearer(const AuthCache& auth_cache,
                    std::vector<server::auth::UserScope> required_scopes)
      : auth_cache_(auth_cache), required_scopes_(std::move(required_scopes)) {}

  [[nodiscard]] AuthCheckResult CheckAuth(
      const server::http::HttpRequest& request,
      server::request::RequestContext& request_context) const override;

  [[nodiscard]] bool SupportsUserAuth() const noexcept override { return true; }

 private:
  const AuthCache& auth_cache_;
  const std::vector<server::auth::UserScope> required_scopes_;
};

}  // namespace samples::pg
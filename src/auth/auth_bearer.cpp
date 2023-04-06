#include "auth_bearer.hpp"

#include <algorithm>
#include <iostream>

namespace auth {

AuthCheckerBearer::AuthCheckResult AuthCheckerBearer::CheckAuth(
    const server::http::HttpRequest& request,
    server::request::RequestContext& request_context) const {
  const auto& auth_value = request.GetHeader("Authorization");

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << auth_value;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;

  if (auth_value.empty()) {
    return AuthCheckResult{AuthCheckResult::Status::kTokenNotFound,
                           {},
                           "Empty 'Authorization' header",
                           server::handlers::HandlerErrorCode::kUnauthorized};
  }

  const auto bearer_sep_pos = auth_value.find(' ');
  if (bearer_sep_pos == std::string::npos ||
      std::string_view{auth_value.data(), bearer_sep_pos} != "Bearer") {
    return AuthCheckResult{
        AuthCheckResult::Status::kTokenNotFound,
        {},
        "'Authorization' header should have 'Bearer some-token' format",
        server::handlers::HandlerErrorCode::kUnauthorized};
  }

  const server::auth::UserAuthInfo::Ticket token{auth_value.data() +
                                                 bearer_sep_pos + 1};
  const auto cache_snapshot = auth_cache_.Get();

  auto it = cache_snapshot->find(token);
  if (it == cache_snapshot->end()) {
    return AuthCheckResult{AuthCheckResult::Status::kForbidden, {}, "Invalid Token"};
  }

  const UserDbInfo& info = it->second;
  for (const auto& scope : required_scopes_) {
    if (std::find(info.scopes.begin(), info.scopes.end(), scope.GetValue()) ==
        info.scopes.end()) {
      return AuthCheckResult{AuthCheckResult::Status::kForbidden,
                             {},
                             "No '" + scope.GetValue() + "' permission"};
    }
  }

  request_context.SetData("name", info.name);
  return {};
}

server::handlers::auth::AuthCheckerBasePtr CheckerFactory::operator()(
    const ::components::ComponentContext& context,
    const server::handlers::auth::HandlerAuthConfig& auth_config,
    const server::handlers::auth::AuthCheckerSettings&) const {
  auto scopes = auth_config["scopes"].As<server::auth::UserScopes>({});
  const auto& auth_cache = context.FindComponent<AuthCache>();
  return std::make_shared<AuthCheckerBearer>(auth_cache, std::move(scopes));
}

}  // namespace samples::pg
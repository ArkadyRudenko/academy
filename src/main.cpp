
#include <userver/utest/using_namespace_userver.hpp>

#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "auth/auth_bearer.hpp"
#include "auth/user_info_cache.hpp"
#include "handlers/auth/login.hpp"
#include "handlers/auth/signup.hpp"
#include "handlers/hello.hpp"
#include "handlers/get_account_info.hpp"

/*
 * auth/login
 * auth/signup
 * account
 */

int main(int argc, char* argv[]) {
  server::handlers::auth::RegisterAuthCheckerFactory(
      "bearer", std::make_unique<auth::CheckerFactory>());

  auto component_list = components::MinimalServerComponentList()
                            .Append<server::handlers::Ping>()
                            .Append<components::TestsuiteSupport>()
                            .Append<components::HttpClient>()
                            .Append<server::handlers::TestsControl>();

  handlers::AppendHello(component_list);
  auth::AppendAuthCache(component_list);
  handlers::auth::AppendUserLogin(component_list);
  handlers::auth::AppendUserSignUp(component_list);
  handlers::AppendGetAccountInfo(component_list);

  return utils::DaemonMain(argc, argv, component_list);
}

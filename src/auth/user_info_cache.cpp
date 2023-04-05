#include "user_info_cache.hpp"

namespace auth {

void AppendAuthCache(userver::components::ComponentList& component_list) {
  component_list.Append<AuthCache>();
}

}  // namespace auth
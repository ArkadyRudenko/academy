#pragma once

#include <cstdint>
#include <iterator>

#include <userver/storages/postgres/io/composite_types.hpp>

namespace pg = userver::storages::postgres;

namespace model {

struct User {
  std::int32_t id;
  std::string full_name;
  std::string tg_id;

  static formats::json::Value ToJson(const User& user) {
    formats::json::ValueBuilder builder;
    builder["id"] = user.id;
    builder["full_name"] = user.full_name;
    builder["tg_id"] = user.tg_id;
    return builder.ExtractValue();
  }
};

}  // namespace model

namespace userver::storages::postgres::io {

template <>
struct CppToUserPg<model::User> {
  static constexpr DBTypeName postgres_name = "academy.new_users";
};

}  // namespace userver::storages::postgres::io

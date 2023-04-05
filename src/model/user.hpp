#pragma once

#include <cstdint>
#include <iterator>

#include <userver/storages/postgres/io/composite_types.hpp>
#include <userver/formats/json.hpp>

namespace pg = userver::storages::postgres;

namespace model {

struct User {
  std::int32_t user_id;
  std::string login;
  std::string password;
  std::string full_name;
  std::string tg_id;

  static userver::formats::json::Value ToJson(const User& user) { // TODO auto-generate json? - see docs
    userver::formats::json::ValueBuilder builder;
    builder["user_id"] = user.user_id;
    builder["login"] = user.login;
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

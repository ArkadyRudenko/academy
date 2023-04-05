#pragma once

#include "userver/utest/using_namespace_userver.hpp"

#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/component.hpp"

namespace db {

const storages::postgres::Query kRegisterUser{
    "INSERT INTO academy.new_users (login, password, full_name, tg_id) "
    "VALUES($1, $2, $3, $4) "
    "RETURNING new_users",
    storages::postgres::Query::Name{"insert_user"},
};

const storages::postgres::Query kGetUserById{
    "SELECT * FROM academy.new_users WHERE user_id = $1",
    storages::postgres::Query::Name{"get_user_by_id"},
};

const storages::postgres::Query kGetUserByLogin{
    "SELECT * FROM academy.new_users WHERE login = $1",
    storages::postgres::Query::Name{"get_user_by_login"},
};

const storages::postgres::Query kInsertAccessToken{
    "INSERT INTO academy.tokens (token, user_id, scopes, name) VALUES($1, $2, "
    "$3, $4) "
    "RETURNING token",
    storages::postgres::Query::Name{"insert_access_token"},
};

const storages::postgres::Query kSelectAccessToken{
    "SELECT token, user_id, scopes, name FROM academy.tokens WHERE user_id = "
    "$1",
    storages::postgres::Query::Name{"select_access_token"},
};

}  // namespace db
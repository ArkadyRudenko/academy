DROP SCHEMA IF EXISTS academy CASCADE;

CREATE SCHEMA IF NOT EXISTS academy;

-- DROP TABLE IF EXISTS users;
-- DROP TABLE IF EXISTS new_users;

CREATE TABLE IF NOT EXISTS academy.users (
    name TEXT PRIMARY KEY,
    count INTEGER DEFAULT(1)
);

CREATE TABLE IF NOT EXISTS academy.new_users (
    user_id SERIAL PRIMARY KEY,
    login VARCHAR(80) NOT NULL,
    password VARCHAR(80) NOT NULL,
    full_name VARCHAR(80) NOT NULL,
    tg_id VARCHAR(80) NOT NULL
);

CREATE TABLE IF NOT EXISTS academy.tokens (
    token TEXT PRIMARY KEY NOT NULL,
    user_id integer NOT NULL,
    scopes TEXT[] NOT NULL,
    name TEXT NOT NULL,
    updated TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
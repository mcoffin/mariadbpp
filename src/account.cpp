//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    Frantisek Boranek 2015,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mariadb++/account.hpp>
#include <mariadb++/conversion_helper.hpp>
#include <utility>
#include "containers.hpp"

using namespace mariadb;

account::account(intercept::types::r_string host_name, intercept::types::r_string user_name,
                 intercept::types::r_string password, intercept::types::r_string schema, u32 port,
                 intercept::types::r_string unix_socket)
    : m_auto_commit(true),
      m_port(port),
      m_host_name(std::move(host_name)),
      m_user_name(std::move(user_name)),
      m_password(std::move(password)),
      m_schema(std::move(schema)),
      m_unix_socket(std::move(unix_socket)) {}

const intercept::types::r_string &account::host_name() const { return m_host_name; }

const intercept::types::r_string &account::user_name() const { return m_user_name; }

const intercept::types::r_string &account::password() const { return m_password; }

const intercept::types::r_string &account::unix_socket() const { return m_unix_socket; }

u32 account::port() const { return m_port; }

const intercept::types::r_string &account::ssl_key() const { return m_ssl_key; }

const intercept::types::r_string &account::ssl_certificate() const { return m_ssl_certificate; }

const intercept::types::r_string &account::ssl_ca() const { return m_ssl_ca; }

const intercept::types::r_string &account::ssl_ca_path() const { return m_ssl_ca_path; }

const intercept::types::r_string &account::ssl_cipher() const { return m_ssl_cipher; }

const intercept::types::r_string &account::schema() const { return m_schema; }

void account::set_schema(const intercept::types::r_string &schema) { m_schema = schema; }

void account::set_ssl(const intercept::types::r_string &key, const intercept::types::r_string &certificate, const intercept::types::r_string &ca,
                      const intercept::types::r_string &ca_path, const intercept::types::r_string &cipher) {
    m_ssl_key = key;
    m_ssl_certificate = certificate;
    m_ssl_ca = ca;
    m_ssl_ca_path = ca_path;
    m_ssl_cipher = cipher;
}

bool account::auto_commit() const { return m_auto_commit; }

void account::set_auto_commit(bool auto_commit) { m_auto_commit = auto_commit; }

const account::map_options_t &account::options() const { return m_options; }

const intercept::types::r_string account::option(const intercept::types::r_string &name) const {
    const map_options_t::const_iterator value = m_options.find(name);

    // return option value if found
    return value == m_options.end() ? intercept::types::r_string() : value->second;
}

void account::set_option(const intercept::types::r_string &name, const intercept::types::r_string &value) {
    m_options[name] = value;
}

void account::clear_options() { m_options.clear(); }

const account::map_connect_options_t &account::connect_options() const { return m_connect_options; }

void account::clear_connect_options() { m_connect_options.clear(); }

void account::set_connect_option(mysql_option option, bool arg) {
    m_connect_options[option] = std::unique_ptr<option_arg>(new option_arg_bool(arg));
}

void account::set_connect_option(mysql_option option, int arg) {
    m_connect_options[option] = std::unique_ptr<option_arg>(new option_arg_int(arg));
}

void account::set_connect_option(mysql_option option, intercept::types::r_string arg) {
    m_connect_options[option] = std::unique_ptr<option_arg>(new option_arg_string(arg));
}


account_ref account::create(intercept::types::r_string host_name, intercept::types::r_string user_name,
                            intercept::types::r_string password, intercept::types::r_string schema, u32 port,
                            intercept::types::r_string unix_socket) {
    return account_ref(new account(std::move(host_name), std::move(user_name), std::move(password),
                                   std::move(schema), port, std::move(unix_socket)));
}

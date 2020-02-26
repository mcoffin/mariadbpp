//
//  M A R I A D B + +
//
//          Copyright Sylvain Rochette Langlois 2013,
//                    The ViaDuck Project 2016 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef _MARIADB_ACCOUNT_HPP_
#define _MARIADB_ACCOUNT_HPP_

#include <string>
#include <map>
#include <mariadb++/types.hpp>
#include <mysql.h>
#include "shared/types.hpp"

namespace mariadb {
class account;
typedef std::shared_ptr<account> account_ref;

class option_arg {
   public:
    virtual ~option_arg() = default;
    virtual const void *value() = 0;
};

#define MAKE_OPTION_ARG(name, type, return_value)                   \
class option_arg_##name : public option_arg {                       \
   public:                                                          \
    explicit option_arg_##name(type arg) : m_value(std::move(arg)) { }  \
    const void *value() override { return return_value; }           \
   protected:                                                       \
    type m_value;                                                   \
}

MAKE_OPTION_ARG(bool, bool, &m_value);
MAKE_OPTION_ARG(int, int, &m_value);
MAKE_OPTION_ARG(string, intercept::types::r_string, m_value.c_str());

/**
 * Class used to store account and connection information used by mariadb::connection when
 * connecting.
 * Note that modifying an account after the connection was established is useless.
 */
class account {
   public:
    typedef std::map<intercept::types::r_string, intercept::types::r_string> map_options_t;
    typedef std::map<mysql_option, std::unique_ptr<option_arg>> map_connect_options_t;

   public:
    /**
     * Destructs the account
     */
    virtual ~account() {}

    /**
     * Gets the name of the host to connect to
     */
    const intercept::types::r_string &host_name() const;

    /**
     * Gets the username to log in with
     */
    const intercept::types::r_string &user_name() const;

    /**
     * Gets the password of the user to log in with
     */
    const intercept::types::r_string &password() const;

    /**
     * Gets the unix socket path to connect to.
     * If this option is set, host and port will be ignored
     */
    const intercept::types::r_string &unix_socket() const;

    /**
     * Gets the path to the key file
     */
    const intercept::types::r_string &ssl_key() const;

    /**
     * Gets the path to the certificate file
     */
    const intercept::types::r_string &ssl_certificate() const;

    /**
     * Gets the path to the certificate authority file
     */
    const intercept::types::r_string &ssl_ca() const;

    /**
     * Gets the path to the directory containing CA intercept::types::r_string
     */
    const intercept::types::r_string &ssl_ca_path() const;

    /**
     * Gets the list of allowed SSL ciphers
     */
    const intercept::types::r_string &ssl_cipher() const;

    /**
     * Gets the port to connect to
     */
    u32 port() const;

    /**
     * Gets the name of the database to open on connect
     */
    const intercept::types::r_string &schema() const;

    /**
     * Sets the name of the database to open on connect
     */
    void set_schema(const intercept::types::r_string &schema);

    /**
     * Set SSL options. All intercept::types::r_string should be PEM format
     *
     * @param key           Path to the key file
     * @param certificate   Path to the certificate file
     * @param ca            Path to the certificate authority file
     * @param ca_path       Path to a directory containing CA intercept::types::r_string
     * @param cipher        List of allowed SSL ciphers. See MariaDB manual for possible values
     */
    void set_ssl(const intercept::types::r_string &key, const intercept::types::r_string &certificate, const intercept::types::r_string &ca,
                 const intercept::types::r_string &ca_path, const intercept::types::r_string &cipher);

    /**
     * Gets the current state of the auto_commit option. This option is turned on by default.
     */
    bool auto_commit() const;

    /**
     * Sets the state of the auto_commit option.
     */
    void set_auto_commit(bool auto_commit);

    /**
     * Gets the current state of the store_result option. When set, the connection uses buffered store
     * (mysql_store_result) instead of unbuffered store (mysql_use_result). This option is turned on by default.
     *
     * Note: Unbuffered store has some restrictions and might lead to unexpected behavior. See the documentation
     * (https://mariadb.com/kb/en/library/mysql_use_result/) for more information.
     */
    bool store_result() const;

    /**
     * Sets the state of the store_result option.
     */
    void set_store_result(bool store_result);

    /**
     * Gets the current value of any named option that was previously set
     *
     * @return Value of the found option or empty string if not found
     */
    const intercept::types::r_string option(const intercept::types::r_string &name) const;

    /**
     * Gets a map of all option key/value pairs previously set
     */
    const map_options_t &options() const;

    /**
     * Sets a named option key/value pair
     */
    void set_option(const intercept::types::r_string &name, const intercept::types::r_string &value);

    /**
     * Deletes all stored key/value pairs of named options
     */
    void clear_options();

    /**
     * Gets a map of all connect option key/value pairs previously set
     */
    const map_connect_options_t &connect_options() const;

    /**
     * Sets a connect option key/value pair with bool argument
     */
    void set_connect_option(mysql_option option, bool arg);
    /**
     * Sets a connect option key/value pair with int argument
     */
    void set_connect_option(mysql_option option, int arg);
    /**
     * Sets a connect option key/value pair with string argument
     */
    void set_connect_option(mysql_option option, intercept::types::r_string arg);

    /**
     * Deletes all stored key/value pairs of named options
     */
    void clear_connect_options();

    /**
     * Create an account
     *
     * @param host_name Hostname to connect to
     * @param user_name Username to log in with
     * @param password  Password for the user to log in with (may be empty)
     * @param schema    Database name to select on connect. Can also be set after connecting
     * @param port      Port of host to connect to (defaults to 3306)
     * @param unix_sock Path of unix socket to connect to. If specified, host and port will be
     * ignored
     */
    static account_ref create(intercept::types::r_string host_name, intercept::types::r_string user_name,
                              intercept::types::r_string password, intercept::types::r_string schema= {},
                              u32 port = 3306, intercept::types::r_string unix_socket = {});

    void addErrorHandler(intercept::types::game_value handler) {
        errorHandlers.emplace_back(std::move(handler));
    }

    bool hasErrorHandler() const { return !errorHandlers.empty(); }

    const auto& getErrorHandlers() const { return errorHandlers; }


   private:
    /**
     * Private account constructor
     */
    account(intercept::types::r_string host_name, intercept::types::r_string user_name, intercept::types::r_string password,
            intercept::types::r_string schema, u32 port, intercept::types::r_string unix_sock);

    bool m_auto_commit = true;
    bool m_store_result = true;
    u32 m_port;
    intercept::types::r_string m_host_name;
    intercept::types::r_string m_user_name;
    intercept::types::r_string m_password;
    intercept::types::r_string m_schema;
    intercept::types::r_string m_unix_socket;
    intercept::types::r_string m_ssl_key;
    intercept::types::r_string m_ssl_certificate;
    intercept::types::r_string m_ssl_ca;
    intercept::types::r_string m_ssl_ca_path;
    intercept::types::r_string m_ssl_cipher;
    map_options_t m_options;
    map_connect_options_t m_connect_options;

    std::vector<intercept::types::game_value> errorHandlers;

};
}

#endif

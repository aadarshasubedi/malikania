/*
 * ServerSettings.h -- server settings
 *
 * Copyright (c) 2013, 2014, 2015 Malikania Authors
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _MALIKANIA_SERVER_SETTINGS_H_
#define _MALIKANIA_SERVER_SETTINGS_H_

/**
 * @file ServerSettings.h
 * @brief Server settings
 */

#include <string>

namespace malikania {

/**
 * @class ServerSettingsNetwork
 * @brief Describe network settings
 *
 * Json description:
 *
 * @code
 * {
 *   "host": "address to bind or * for any",
 *   "port": port number
 * }
 * @endcode
 */
class ServerSettingsNetwork {
public:
	std::string host{"*"};
	int port{36000};
};

/**
 * @class ServerSettingSsl
 * @brief Describe SSL settings
 *
 * Json description:
 *
 * @code
 * {
 *   "private-key": "path to the private key file",
 *   "certificate": "certificate file"
 *   "port": port number
 * }
 * @endcode
 */
class ServerSettingsSsl {
public:
	std::string privateKey;
	std::string certificate;
	int port{36001};
};

/**
 * @class ServerSettingDatabase
 * @brief Database settings
 *
 * Json description:
 *
 * @code
 * {
 *   "host": "database hostname",
 *   "username": "user to use",
 *   "dbname": "database name",
 *   "password": "the password",
 *   "port": port number
 * }
 * @endcode
 */
class ServerSettingsDatabase {
public:
	std::string host{"localhost"};
	std::string username;
	std::string dbname;
	std::string password;
	int port{0};
};

/**
 * @class ServerSettings
 * @brief Pack all settings
 */
class ServerSettings {
public:
	ServerSettingsNetwork network;
	ServerSettingsDatabase database;
	ServerSettingsSsl ssl;
};

} // !malikania

#endif // !_MALIKANIA_SERVER_SETTINGS_H_


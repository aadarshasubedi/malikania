/*
 * ServerLoaderDirectory.cpp -- load a server from a directory
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

#include <cerrno>
#include <cstring>

#include "ServerLoaderDirectory.h"

namespace malikania {

std::string jsonAsString(const JsonValue &value)
{
	std::string result;

	switch (value.typeOf()) {
	case JsonType::False:
		result = "false";
		break;
	case JsonType::True:
		result = "true";
		break;
	case JsonType::Integer:
		result = std::to_string(value.toInteger());
		break;
	case JsonType::Real:
		result = std::to_string(value.toReal());
		break;
	case JsonType::String:
		result = value.toString();
		break;
	default:
		break;
	}

	return result;
}

inline JsonValue jsonOr(const JsonObject &object, const std::string &key, JsonValue def)
{
	return (object.contains(key)) ? object[key] : def;
}

ServerSettingsNetwork readNetwork(const JsonObject &object)
{
	ServerSettingsNetwork network;

	network.port = jsonOr(object, "port", network.port).toInteger();
	network.host = jsonOr(object, "host", network.host).toString();

	return network;
}

ServerSettingsDatabase readDatabase(const JsonObject &object)
{
	ServerSettingsDatabase db;

	db.host = jsonOr(object, "host", db.host).toString();
	db.username = jsonOr(object, "username", db.username).toString();
	db.dbname = jsonOr(object, "dbname", db.dbname).toString();
	db.password = jsonOr(object, "password", db.password).toString();
	db.port = jsonOr(object, "port", db.port).toInteger();

	return db;
}

ServerSettingsSsl readSsl(const JsonObject &object)
{
	ServerSettingsSsl ssl;

	ssl.port = jsonOr(object, "port", ssl.port).toInteger();
	ssl.privateKey = jsonOr(object, "private-key", ssl.privateKey).toString();
	ssl.certificate = jsonOr(object, "certificate", ssl.certificate).toString();

	return ssl;
}

ServerSettings ServerLoaderDirectory::serverSettings()
{
	std::string path = m_path + "/server.json";
	std::ifstream file(path);

	if (!file.is_open()) {
		throw std::runtime_error(path + ": " + std::strerror(errno));
	}

	JsonDocument document(file);
	JsonObject object = document.toObject();
	ServerSettings settings;

	if (!object.isObject()) {
		throw std::runtime_error("invalid file");
	}
	if (!object.contains("database")) {
		throw std::runtime_error("missing `database' property");
	}
	if (!object.contains("network")) {
		throw std::runtime_error("missing `network' property");
	}
	if (!object.contains("ssl")) {
		throw std::runtime_error("missing `ssl' property");
	}

	settings.database = readDatabase(object["database"].toObject());
	settings.network = readNetwork(object["network"].toObject());
	settings.ssl = readSsl(object["ssl"].toObject());

	return settings;
}

} // !malikania

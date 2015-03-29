/*
 * DirectoryLoader.h -- wrapper to load directories
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

#ifndef _MALIKANIA_DIRECTORY_LOADER_H_
#define _MALIKANIA_DIRECTORY_LOADER_H_

/**
 * @file DirectoryLoader.h
 * @brief Help loading games from directories
 */

#include <cerrno>
#include <cstring>
#include <stdexcept>

#include "Loader.h"
#include "Json.h"

namespace malikania {

/**
 * @class DirectoryLoader
 * @brief Base class for directories loader
 *
 * This class provides a gameSettings() implementation for a directory. It is
 * implemented as a template so that server and client can request to derive
 * from ServerLoader and ClientLoader.
 */
template <typename Interface>
class DirectoryLoader : public Interface {
protected:
	std::string m_path;

	JsonValue requires(const JsonObject &object, const std::string &name, JsonType type) const;

	JsonValue optional(const JsonObject &object, const std::string &name, JsonType type, JsonValue def = JsonValue()) const;

public:
	/**
	 * Create the loader from the specified path.
	 *
	 * @param path the path
	 */
	DirectoryLoader(std::string path) noexcept;

	/**
	 * Get the game settings.
	 *
	 * @return the game settings
	 */
	GameSettings gameSettings();
};

template <typename Interface>
JsonValue DirectoryLoader<Interface>::requires(const JsonObject &object, const std::string &name, JsonType type) const
{
	if (!object.contains(name))
		throw std::runtime_error("missing `" + name + "' field");

	JsonValue value = object[name];

	if (value.typeOf() != type)
		throw std::runtime_error("invalid type given");

	return value;
}

template <typename Interface>
JsonValue DirectoryLoader<Interface>::optional(const JsonObject &object, const std::string &name, JsonType type, JsonValue def) const
{
	JsonValue value = object[name];

	if (value.typeOf () != type)
		return def;

	return value;
}

template <typename Interface>
DirectoryLoader<Interface>::DirectoryLoader(std::string path) noexcept
	: m_path(std::move(path))
{
}

template <typename Interface>
GameSettings DirectoryLoader<Interface>::gameSettings()
{
	std::string path = m_path + "/game.json";
	std::ifstream file(path);

	if (!file.is_open())
		throw std::runtime_error(path + ": " + std::strerror(errno));

	JsonDocument document = JsonDocument(file);

	if (!document.isObject())
		throw std::runtime_error("invalid game.json file");

	JsonObject object = document.toObject();
	GameSettings game;

	// Required fields
	game.name = requires(object, "name", JsonType::String).toString();
	game.version = requires(object, "version", JsonType::String).toString();
	game.requires = requires(object, "requires", JsonType::String).toString();

	JsonArray authors = requires(object, "authors", JsonType::Array).toArray();

	if (authors.size() == 0) {
		throw std::runtime_error("field `authors' needs at least one element");
	}

	for (const JsonValue &v : authors) {
		if (!v.isString()) {
			throw std::runtime_error("author element is not a string");
		}

		game.authors.push_back(v.toString());
	}

	// Optional fields
	game.license = optional(object, "license", JsonType::String).toString();

	return game;
}

} // !malikania

#endif // !_MALIKANIA_DIRECTORY_LOADER_H_


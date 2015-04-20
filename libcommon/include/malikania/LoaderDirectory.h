/*
 * LoaderDirectory.h -- wrapper to load directories
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

#ifndef _MALIKANIA_LOADER_DIRECTORY_H_
#define _MALIKANIA_LOADER_DIRECTORY_H_

/**
 * @file LoaderDirectory.h
 * @brief Help loading games from directories
 */

#include <cerrno>
#include <cstring>
#include <stdexcept>

#include "Json.h"
#include "GameSettings.h"

namespace malikania {

/**
 * @class LoaderDirectory
 * @brief Base class for directories loader
 *
 * This class provides a gameSettings() implementation for a directory. It is
 * implemented as a template so that server and client can request to derive
 * from ServerLoader and ClientLoader.
 */
class LoaderDirectory {
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
	LoaderDirectory(std::string path) noexcept;

	/**
	 * Virtual destructor defaulted.
	 */
	virtual ~LoaderDirectory() = default;

	/**
	 * Get the game settings.
	 *
	 * @return the game settings
	 */
	GameSettings gameSettings();
};

} // !malikania

#endif // !_MALIKANIA_LOADER_DIRECTORY_H_


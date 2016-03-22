/*
 * ResourcesLoader.h -- load shared resources files
 *
 * Copyright (c) 2013-2016 Malikania Authors
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

#ifndef _MALIKANIA_RESOURCES_LOADER_H_
#define _MALIKANIA_RESOURCES_LOADER_H_

#include <string>
#include <unordered_map>

#include "Common.h"
#include "Json.h"
#include "ResourcesLocator.h"

namespace malikania {

class Game;

/**
 * @class ResourcesLoader
 * @brief Open resources files using a ResourcesLocator.
 *
 * This class is used to load resources files that are common to the server and the client.
 *
 * @see ResourcesLoaderClient
 * @see ResourcesLoaderServer
 */
class MALIKANIA_COMMON_EXPORT ResourcesLoader {
private:
	ResourcesLocator &m_locator;

protected:
	/**
	 * Check that an object has the specified properties of the given type.
	 *
	 * Throws an error when any of the property is missing or not the correct type.
	 *
	 * You can use this function when you have lot of properties to extract, otherwise, you can use one of the
	 * require* or get* functions to avoid performances overhead.
	 *
	 * @pre object.isObject()
	 * @param id the resource id
	 * @param object the object
	 * @param props the properties
	 * @throw std::runtime_error when a property is missing / invalid
	 */
	void requires(const std::string &id,
		      const json::Value &object,
		      const std::unordered_map<std::string, json::Type> props) const;

	/**
	 * Require a string.
	 *
	 * @pre object.isObject()
	 * @param id the resource id
	 * @param object the object
	 * @param property the property
	 * @return the string
	 * @throw std::runtime_error if the property is not a string or missing
	 */
	std::string requireString(const std::string &id, const json::Value &object, const std::string &property) const;

public:
	/**
	 * Construct the ResourcesLoader.
	 *
	 * @param locator the locator
	 */
	ResourcesLoader(ResourcesLocator &locator);

	/**
	 * Virtual destructor defaulted.
	 */
	virtual ~ResourcesLoader() = default;

	/**
	 * Get the underlying locator.
	 *
	 * @return the locator
	 */
	inline ResourcesLocator &locator() noexcept
	{
		return m_locator;
	}

	/**
	 * Load a game.
	 *
	 * @return the game
	 * @throw std::runtime_error on errors
	 */
	virtual Game loadGame() const;
};

} // !malikania

#endif // !_MALIKANIA_RESOURCES_LOADER_H_

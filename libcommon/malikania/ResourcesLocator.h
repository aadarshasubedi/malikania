/*
 * ResourcesLocator.h -- file and stream loader
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

#ifndef _MALIKANIA_RESOURCES_LOCATOR_H_
#define _MALIKANIA_RESOURCES_LOCATOR_H_

#include <string>
#include <memory>
#include <istream>

#include "Common.h"

namespace malikania {

/**
 * @class ResourcesLocator
 * @brief Load files from directories and zip.
 */
class MALIKANIA_COMMON_EXPORT ResourcesLocator {
public:
	/**
	 * Read a whole resource as a string.
	 *
	 * @param id the resource id
	 * @return the string
	 * @throw std::runtime_error on any errors
	 */
	virtual std::string read(const std::string &id) = 0;

	/**
	 * Open a resource as a stream.
	 *
	 * @param id the resource id
	 * @return the stream
	 * @throw std::runtime_error on any errors
	 */
	virtual std::unique_ptr<std::istream> open(const std::string &id) = 0;
};

/**
 * @class ResourcesLocatorDirectory
 * @brief Load a game from a directory.
 */
class ResourcesLocatorDirectory : public ResourcesLocator {
private:
	std::string m_path;

public:
	/**
	 * Load the game from the directory.
	 *
	 * @param path the base directory
	 */
	ResourcesLocatorDirectory(std::string path) noexcept;

	/**
	 * @copydoc ResourcesLocator::read
	 */
	std::string read(const std::string &id) override;

	/**
	 * @copydoc ResourcesLocator::open
	 */
	std::unique_ptr<std::istream> open(const std::string &id) override;
};

} // !malikania

#endif // !_MALIKANIA_RESOURCES_LOCATOR_H_

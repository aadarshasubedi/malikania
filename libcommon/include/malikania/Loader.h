/*
 * Loader.h -- wrapper to load directories
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

#ifndef _MALIKANIA_LOADER_H_
#define _MALIKANIA_LOADER_H_

namespace malikania {

namespace json {

class Document;

} // !json

/**
 * @class LoaderDirectory
 * @brief Load a game from a directory.
 */
class LoaderDirectory {
private:
	std::string m_path;

public:
	/**
	 * Load the game from the directory.
	 *
	 * @param path the base directory
	 */
	inline LoaderDirectory(std::string path) noexcept
		: m_path{std::move(path)}
	{
	}

	/**
	 * Get the game metadata.
	 *
	 * @return the metadata
	 * @throw any exception on failures
	 */
	json::Document metadata() const;
};

} // !malikania

#endif // !_MALIKANIA_LOADER_H_


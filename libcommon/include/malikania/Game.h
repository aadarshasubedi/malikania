/*
 * Game.h -- basic game class
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

#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <vector>

namespace malikania {

namespace json {

class Document;

} // !json

/**
 * @class Game
 * @brief Basic game class.
 */
class Game {
private:
	std::string m_name;
	std::vector<std::string> m_authors;
	std::string m_license;
	std::string m_version;
	std::string m_requires;

public:
	/**
	 * Load the game metadata.
	 *
	 * @param document the game metadata
	 */
	Game(const json::Document &document);

	/**
	 * Get the game name.
	 *
	 * @return the name
	 */
	inline const std::string &name() const noexcept
	{
		return m_name;
	}

	/**
	 * Get the authors.
	 *
	 * @return the authors
	 */
	inline const std::vector<std::string> &authors() const noexcept
	{
		return m_authors;
	}

	/**
	 * Get the license.
	 *
	 * @return the license
	 */
	inline const std::string &license() const noexcept
	{
		return m_license;
	}

	/**
	 * Get the license.
	 *
	 * @return the license
	 */
	inline const std::string &version() const noexcept
	{
		return m_version;
	}

	/**
	 * Get the engine version required to run the game.
	 *
	 * @return the version required
	 */
	inline const std::string &requires() const noexcept
	{
		return m_requires;
	}
};

} // !malikania

#endif // !_GAME_H_

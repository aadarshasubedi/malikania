/*
 * Loader.h -- abstract base class for loading games
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

/**
 * @file Loader.h
 * @brief Abstract interface class for loading game
 */

#include "GameSettings.h"

namespace malikania {

/**
 * @class Loader
 * @brief Abstract interface class for loading game
 * @see ServerLoader
 * @see ClientLoader
 */
class Loader {
public:
	/**
	 * Default constructor.
	 */
	Loader() = default;

	/**
	 * Default destructor.
	 */
	virtual ~Loader() = default;

	/**
	 * Get the game data definition.
	 *
	 * @return the game
	 */
	virtual GameSettings gameSettings() = 0;
};

} // !malikania

#endif // !_MALIKANIA_LOADER_H_


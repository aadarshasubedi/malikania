/*
 * ServerApplication.h -- server application
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

#ifndef _MALIKANIA_SERVER_APPLICATION_H_
#define _MALIKANIA_SERVER_APPLICATION_H_

/**
 * @file ServerApplication.h
 * @brief Load the server
 */

#include <memory>
#include <type_traits>

#include <malikania/Application.h>

namespace malikania {

class GameSettings;
class ServerSettings;

/**
 * @class ServerApplication
 * @brief Class to start the server
 */
class ServerApplication : public Application {
private:
	void run(const GameSettings &, const ServerSettings &);

public:
	using Application::Application;

	/**
	 * Load the server with the specified loader.
	 *
	 * @param loader the loader
	 */
	template <typename Loader>
	void run(Loader &&loader)
	{
		run(loader.gameSettings(), loader.serverSettings());
	}
};

} // !malikania

#endif // !_MALIKANIA_SERVER_APPLICATION_H_


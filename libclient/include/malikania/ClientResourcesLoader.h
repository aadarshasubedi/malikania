/*
 * ClientResourcesLoader.h -- load shared resources files for the client
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

#ifndef _MALIKANIA_CLIENT_RESOURCES_LOADER_H_
#define _MALIKANIA_CLIENT_RESOURCES_LOADER_H_

#include <malikania/ResourcesLoader.h>

namespace malikania {

class Image;
class Sprite;
class Window;

/**
 * @class ClientResourcesLoader
 * @brief Load client resources.
 */
class ClientResourcesLoader : public ResourcesLoader {
private:
	Window &m_window;

public:
	/**
	 * Client resources loader constructor.
	 *
	 * The window is required because some of the resources require it.
	 *
	 * @param window the window
	 * @param locator the resources locator
	 */
	inline ClientResourcesLoader(Window &window, ResourcesLocator &locator)
		: ResourcesLoader(locator)
		, m_window(window)
	{
	}

	/**
	 * Load an image.
	 *
	 * @param id the resource id
	 * @return the image
	 * @throw std::runtime_error on errors
	 */
	virtual Image loadImage(const std::string &id);

	/**
	 * Load a sprite.
	 *
	 * @param id the resource id
	 * @return the sprite
	 * @throw std::runtime_error on errors
	 */
	virtual Sprite loadSprite(const std::string &id);
};

} // !malikania

#endif // !_MALIKANIA_CLIENT_RESOURCES_LOADER_H_

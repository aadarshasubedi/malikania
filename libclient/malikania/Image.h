/*
 * Image.h -- image object
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

#ifndef _MALIKANIA_IMAGE_H_
#define _MALIKANIA_IMAGE_H_

/**
 * @file Image.h
 * @brief Images.
 */

#include <memory>
#include <string>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/ImageSdl.h"
#endif

#include <malikania/Size.h>
#include <malikania/Rectangle.h>

#include "CommonClient.h"
#include "Point.h"

namespace malikania {

class Window;

/**
 * @class Image
 * @brief Image object.
 */
class MALIKANIA_CLIENT_EXPORT Image {
private:
	BackendImage m_backend;

public:
	/**
	 * Construct an image from the binary data.
	 *
	 * @param window the window
	 * @param data the data
	 */
	inline Image(Window &window, const std::string &data)
		: m_backend(*this, window, data)
	{
	}

	/**
	 * Get the underlying backend.
	 *
	 * @return the backend
	 */
	inline BackendImage &backend() noexcept
	{
		return m_backend;
	}

	/**
	 * Get the image size.
	 *
	 * @return the size
	 */
	inline const Size &size() const noexcept
	{
		return m_backend.size();
	}

	/**
	 * Draw the image to the window.
	 *
	 * @param window the window
	 * @param position the position
	 */
	inline void draw(Window &window, const Point &position = {0, 0})
	{
		m_backend.draw(window, position);
	}

	/**
	 * Overloaded function.
	 *
	 * @param window the window
	 * @param source the source to clip
	 * @param target the target destination
	 */
	inline void draw(Window &window, const Rectangle &source, const Rectangle &target)
	{
		m_backend.draw(window, source, target);
	}
};

} // !malikania

#endif // !_MALIKANIA_IMAGE_H_

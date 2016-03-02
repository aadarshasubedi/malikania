/*
 * Sprite.h -- image sprite
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

#ifndef _MALIKANIA_SPRITE_H_
#define _MALIKANIA_SPRITE_H_

/**
 * @file Sprite.h
 * @brief Sprite description.
 */

#include <malikania/Json.h>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/SpriteSdl.h"
#endif

#include "Image.h"

namespace malikania {

class Window;

/**
 * @class Sprite
 * @brief A Sprite is an image divided into cells.
 */
class Sprite {
private:
	BackendSprite m_backend;
	Image m_image;
	Size m_cell;
	Size m_size;
	Size m_space;
	Size m_margin;

public:
	/**
	 * Construct a sprite.
	 *
	 * @param image the image to use
	 * @param cell size of cell in the image
	 * @param size the sprite size (if 0, taken from the image)
	 * @param space the optional space between cells
	 * @param margin the optional space from borders
	 */
	Sprite(Image image, Size cell, Size size = { 0, 0 }, Size space = { 0, 0 }, Size margin = { 0, 0 }) noexcept;

	/**
	 * Get the underlying image.
	 *
	 * @return the image
	 */
	inline const Image &image() const noexcept
	{
		return m_image;
	}

	/**
	 * Overloaded function.
	 *
	 * @return the image
	 */
	inline Image &image() noexcept
	{
		return m_image;
	}

	inline const Size &cell() const noexcept
	{
		return m_cell;
	}

	inline const Size &space() const noexcept
	{
		return m_space;
	}

	inline const Size &margin() noexcept
	{
		return m_margin;
	}

	inline BackendSprite &backend() noexcept
	{
		return m_backend;
	}

	void draw(Window &window, int index, const Rectangle &rectangle);
};

} // !malikania

#endif // !_MALIKANIA_SPRITE_H_

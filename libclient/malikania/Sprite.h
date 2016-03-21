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

#include "Image.h"

namespace malikania {

class Point;
class Window;

/**
 * @class Sprite
 * @brief A Sprite is an image divided into cells.
 */
class Sprite {
private:
	Image m_image;
	Size m_cell;
	Size m_margin;
	Size m_space;
	Size m_size;
	unsigned m_rows;
	unsigned m_columns;

public:
	/**
	 * Construct a sprite.
	 *
	 * @pre cell must not have height or width null
	 * @param image the image to use
	 * @param cell size of cell in the image
	 * @param margin the optional space from borders
	 * @param space the optional space between cells
	 * @param size the sprite size (if 0, taken from the image)
	 */
	Sprite(Image image, Size cell, Size margin = { 0, 0 }, Size space = { 0, 0 }, Size size = { 0, 0 }) noexcept;

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

	/**
	 * Get the cell size.
	 *
	 * @return the cell size
	 */
	inline const Size &cell() const noexcept
	{
		return m_cell;
	}

	/**
	 * Get the margin size.
	 *
	 * @return the margin size
	 */
	inline const Size &margin() noexcept
	{
		return m_margin;
	}

	/**
	 * Get the space size.
	 *
	 * @return the space size
	 */
	inline const Size &space() const noexcept
	{
		return m_space;
	}

	/**
	 * Get the number of rows in the grid.
	 *
	 * @return the number of rows
	 */
	inline unsigned rows() const noexcept
	{
		return m_rows;
	}

	/**
	 * Get the number of columns in the grid.
	 *
	 * @return the number of columns
	 */
	inline unsigned columns() const noexcept
	{
		return m_columns;
	}

	/**
	 * Draw the sprite into the window at the specified position.
	 *
	 * @pre cell < rows() * columns()
	 * @param window the window
	 * @param cell the cell index
	 * @param position the position in the window
	 */
	void draw(Window &window, unsigned cell, const Point &position);
};

} // !malikania

#endif // !_MALIKANIA_SPRITE_H_

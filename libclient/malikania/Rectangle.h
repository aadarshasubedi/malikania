/*
 * Rectangle.h -- rectangle description
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

#ifndef _MALIKANIA_RECTANGLE_H_
#define _MALIKANIA_RECTANGLE_H_

/**
 * @file Rectangle.h
 * @brief Rectangle description.
 */

#include "CommonClient.h"

namespace malikania {

/**
 * @class Rectangle
 * @brief Rectangle description.
 *
 * A rectangle has coordinates (x, y) and dimensions (width, height).
 *
 * They are commonly used for clipping images into the window.
 */
class MALIKANIA_CLIENT_EXPORT Rectangle {
private:
	int m_x;
	int m_y;
	unsigned m_width;
	unsigned m_height;

public:
	/**
	 * Construct a rectangle.
	 *
	 * @param x the x position
	 * @param y the y position
	 * @param width the width
	 * @param height the height
	 */
	inline Rectangle(int x = 0, int y = 0, unsigned width = 0, unsigned height = 0) noexcept
		: m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{
	}

	/**
	 * Get the x position.
	 *
	 * @return the x position
	 */
	inline int x() const noexcept
	{
		return m_x;
	}

	/**
	 * Get the y position.
	 *
	 * @return the y position
	 */
	inline int y() const noexcept
	{
		return m_y;
	}

	/**
	 * Get the rectangle width.
	 *
	 * @return the width
	 */
	inline unsigned width() const noexcept
	{
		return m_width;
	}

	/**
	 * Get the rectangle height.
	 *
	 * @return the height
	 */
	inline unsigned height() const noexcept
	{
		return m_height;
	}

	/**
	 * Check if the rectangle has null dimensions.
	 *
	 * @return true if weight and height are 0
	 */
	inline bool isNull() const noexcept
	{
		return m_width == 0 && m_height == 0;
	}
};

/**
 * Compare equality.
 *
 * @param r1 the first rectangle
 * @param r2 the second rectangle
 * @return true if they equal
 */
inline bool operator==(const Rectangle &r1, const Rectangle &r2) noexcept
{
	return r1.x() == r2.x() && r1.y() == r2.y() && r1.width() == r2.width() && r1.height() == r2.height();
}

/**
 * Compare equality.
 *
 * @param r1 the first rectangle
 * @param r2 the second rectangle
 * @return false if they equal
 */
inline bool operator!=(const Rectangle &r1, const Rectangle &r2) noexcept
{
	return !(r1 == r2);
}

} // !malikania

#endif // !_MALIKANIA_RECTANGLE_H_

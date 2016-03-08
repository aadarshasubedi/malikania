/*
 * Point.h -- point description
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

#ifndef _MALIKANIA_POINT_H_
#define _MALIKANIA_POINT_H_

/**
 * @file Point.h
 * @brief Point description.
 */

namespace malikania {

/**
 * @class Point
 * @brief Point coordinate.
 */
class Point {
private:
	int m_x;
	int m_y;

public:
	/**
	 * Construct a point.
	 *
	 * @param x the x
	 * @param y the y
	 */
	inline Point(int x = 0, int y = 0) noexcept
		: m_x(x)
		, m_y(y)
	{
	}

	/**
	 * Get the x position.
	 *
	 * @return the x
	 */
	inline int x() const noexcept
	{
		return m_x;
	}

	/**
	 * Get the y position.
	 *
	 * @return the y
	 */
	inline int y() const noexcept
	{
		return m_y;
	}
};

/**
 * Compare equality.
 *
 * @param p1 the first point
 * @param p2 the second point
 * @return true if they equal
 */
inline bool operator==(const Point &p1, const Point &p2) noexcept
{
	return p1.x() == p2.x() && p1.y() == p2.y();
}

/**
 * Compare equality.
 *
 * @param p1 the first point
 * @param p2 the second point
 * @return false if they equal
 */
inline bool operator!=(const Point &p1, const Point &p2) noexcept
{
	return !(p1 == p2);
}

} // !malikania

#endif // !_MALIKANIA_POINT_H_

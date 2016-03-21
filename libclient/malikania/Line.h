/*
 * Line.h -- line description
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

#ifndef _MALIKANIA_LINE_H_
#define _MALIKANIA_LINE_H_

/**
 * @file Line.h
 * @brief Line description.
 */

namespace malikania {

/**
 * @class Line
 * @brief Line description.
 *
 * A line has an origin (x, y) and a destination (x, y).
 */
class Line {
private:
	int m_x1;
	int m_y1;
	int m_x2;
	int m_y2;

public:
	/**
	 * Construct a line.
	 *
	 * @param x1 the first x
	 * @param y1 the first y
	 * @param x2 the second x
	 * @param y2 the second y
	 */
	inline Line(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0) noexcept
		: m_x1(x1)
		, m_y1(y1)
		, m_x2(x2)
		, m_y2(y2)
	{
	}

	/**
	 * Get the first x.
	 *
	 * @return the x1
	 */
	inline int x1() const noexcept
	{
		return m_x1;
	}

	/**
	 * Get the first y.
	 *
	 * @return the y1
	 */
	inline int y1() const noexcept
	{
		return m_y1;
	}

	/**
	 * Get the second x.
	 *
	 * @return the x2
	 */
	inline int x2() const noexcept
	{
		return m_x2;
	}

	/**
	 * Get the second y.
	 *
	 * @return the y2
	 */
	inline int y2() const noexcept
	{
		return m_y2;
	}
};

/**
 * Compare equality.
 *
 * @param l1 the first line
 * @param l2 the second line
 * @return true if they equal
 */
inline bool operator==(const Line &l1, const Line &l2) noexcept
{
	return l1.x1() == l2.x1() && l1.x2() == l2.x2() && l1.y1() == l2.y1() && l1.y2() == l2.y2();
}

/**
 * Compare equality.
 *
 * @param l1 the first line
 * @param l2 the second line
 * @return false if they equal
 */
inline bool operator!=(const Line &l1, const Line &l2) noexcept
{
	return !(l1 == l2);
}

} // !malikania

#endif // !_MALIKANIA_LINE_H_

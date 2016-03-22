/*
 * Size.h -- size description
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

#ifndef _MALIKANIA_SIZE_H_
#define _MALIKANIA_SIZE_H_

#include "CommonClient.h"

namespace malikania {

/**
 * @class Size
 * @brief Size description.
 */
class MALIKANIA_CLIENT_EXPORT Size {
private:
	unsigned m_width;
	unsigned m_height;

public:
	/**
	 * Constructor.
	 *
	 * @param width the size width
	 * @param height the size height
	 */
	inline Size(unsigned width = 0, unsigned height = 0) noexcept
		: m_width(width)
		, m_height(height)
	{
	}

	/**
	 * Get the width.
	 *
	 * @return the width
	 */
	inline unsigned width() const noexcept
	{
		return m_width;
	}

	/**
	 * Get the height.
	 *
	 * @return the height
	 */
	inline unsigned height() const noexcept
	{
		return m_height;
	}

	/**
	 * Check if the size is 0, 0.
	 *
	 * @return true if height and width are 0
	 */
	inline bool isNull() const noexcept
	{
		return m_height == 0 && m_width == 0;
	}
};

/**
 * Compare equality.
 *
 * @param s1 the first size
 * @param s2 the second size
 * @return true if they equal
 */
inline bool operator==(const Size &s1, const Size &s2) noexcept
{
	return s1.width() == s2.width() && s1.height() == s2.height();
}

/**
 * Compare equality.
 *
 * @param s1 the first size
 * @param s2 the second size
 * @return false if they equal
 */
inline bool operator!=(const Size &s1, const Size &s2) noexcept
{
	return !(s1 == s2);
}

} // !malikania

#endif // !_MALIKANIA_SIZE_H_

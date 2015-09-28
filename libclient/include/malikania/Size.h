/*
 * Size.h -- object size description
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

#ifndef _MALIKANIA_SIZE_H_
#define _MALIKANIA_SIZE_H_

#include <malikania/Js.h>

namespace malikania {

/**
 * @class Size
 * @brief Describe a size
 */
class Size {
public:
	/**
	 * The width.
	 */
	int width{0};

	/**
	 * The height.
	 */
	int height{0};
};

/**
 * Check if two size are identical.
 *
 * @param lhs the left side
 * @param rhs the right side
 * @return true if they equals
 */
constexpr bool operator==(const Size &lhs, const Size &rhs) noexcept
{
	return lhs.width == rhs.width && lhs.height == rhs.height;
}

/**
 * Check if two size are different
 *
 * @param lhs the left side
 * @param rhs the right side
 * @return true if they are different
 */
constexpr bool operator!=(const Size &lhs, const Size &rhs) noexcept
{
	return !(lhs == rhs);
}

namespace js {

/**
 * @class TypeInfo<Size>
 * @brief Bring JavaScript support for Size
 */
template <>
class TypeInfo<Size> {
public:
	/**
	 * Push a Size object into the context.
	 *
	 * @param size the size object
	 */
	static void push(Context &ctx, const Size &size);

	/**
	 * Get a size object from the context.
	 *
	 * @param index the index
	 * @return the size
	 */
	static Size get(Context &ctx, duk_idx_t index);

	/**
	 * Require a size object from the context.
	 *
	 * @param ctx the context
	 * @param index the index
	 * @param size the destination
	 */
	static Size require(Context &ctx, duk_idx_t index);
};

} // !js

} // !malikania

#endif // !_MALIKANIA_SIZE_H_

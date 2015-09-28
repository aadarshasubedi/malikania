/*
 * Rectangle.h -- describe a rectangle
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

#ifndef _MALIKANIA_RECTANGLE_H_
#define _MALIKANIA_RECTANGLE_H_

#include <malikania/Js.h>

namespace malikania {

/**
 * @class Rectangle
 * @brief Describe a rectangle
 */
class Rectangle {
public:
	int x{0};		//!< Position in X
	int y{0};		//!< Position in Y
	int width{0};		//!< The width
	int height{0};		//!< The height
};

namespace js {

template <>
class TypeInfo<Rectangle> {
public:
	static void push(Context &ctx, const Rectangle &rect);
	static Rectangle get(Context &ctx, duk_idx_t index);
};

} // !js

} // !malikania

#endif // !_MALIKANIA_RECTANGLE_H_

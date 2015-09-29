/*
 * Line.h -- describe a 2D line
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

#ifndef _MALIKANIA_LINE_H_
#define _MALIKANIA_LINE_H_

#include <malikania/Js.h>

namespace malikania {

/**
 * @class Line
 * @brief 2D line
 */
class Line {
public:
	int x1{0};	//!< First position in X
	int y1{0};	//!< First position in Y
	int x2{0};	//!< Second position in X
	int y2{0};	//!< Second position in Y
};

namespace js {

template <>
class TypeInfo<Line> {
public:
	static void push(Context &ctx, const Line &line);
	static Line get(Context &ctx, duk_idx_t index);
};

} // !js

} // !malikania

#endif // LINE_H

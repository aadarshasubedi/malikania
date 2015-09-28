/*
 * Rectangle.cpp -- describe a rectangle
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

#include "Rectangle.h"

namespace malikania {

namespace js {

void TypeInfo<Rectangle>::push(Context &ctx, const Rectangle &rect)
{
	ctx.push(Object{});
	ctx.setObject(-1, "x", rect.x);
	ctx.setObject(-1, "y", rect.y);
	ctx.setObject(-1, "width", rect.width);
	ctx.setObject(-1, "height", rect.height);
}

Rectangle TypeInfo<Rectangle>::get(Context &ctx, duk_idx_t index)
{
	return Rectangle{
		ctx.getObject<int>(index, "x"),
		ctx.getObject<int>(index, "y"),
		ctx.getObject<int>(index, "width"),
		ctx.getObject<int>(index, "height")
	};
}

} // !js

} // !malikania

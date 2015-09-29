/*
 * Line.cpp -- describe a 2D line
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

#include "Line.h"

namespace malikania {

namespace js {

void TypeInfo<Line>::push(Context &ctx, const Line &line)
{
	ctx.push(Object{});
	ctx.setObject(-1, "x1", line.x1);
	ctx.setObject(-1, "y1", line.y1);
	ctx.setObject(-1, "x2", line.x2);
	ctx.setObject(-1, "y2", line.y2);
}

Line TypeInfo<Line>::get(Context &ctx, duk_idx_t index)
{
	return Line{
		ctx.getObject<int>(index, "x1"),
		ctx.getObject<int>(index, "y1"),
		ctx.getObject<int>(index, "x2"),
		ctx.getObject<int>(index, "y2")
	};
}

} // !js

} // !malikania

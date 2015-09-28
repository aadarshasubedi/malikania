
/*
 * main.cpp -- test JavaScript Point
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

#include <gtest/gtest.h>

#include <malikania/Rectangle.h>

using namespace malikania;

TEST(PointTest, simple)
{
	js::Context ctx;

	ctx.push(Rectangle{10, 20, 640, 480});

	ASSERT_EQ(10, ctx.get<Rectangle>(-1).x);
	ASSERT_EQ(20, ctx.get<Rectangle>(-1).y);
	ASSERT_EQ(640, ctx.get<Rectangle>(-1).width);
	ASSERT_EQ(480, ctx.get<Rectangle>(-1).height);
}

TEST(PointTest, fromScriptFull)
{
	js::Context ctx;

	ctx.evalString("rect = { x: -45, y: -80, width: 640, height: 480 };");

	ASSERT_EQ(-45, ctx.getGlobal<Rectangle>("rect").x);
	ASSERT_EQ(-80, ctx.getGlobal<Rectangle>("rect").y);
	ASSERT_EQ(640, ctx.getGlobal<Rectangle>("rect").width);
	ASSERT_EQ(480, ctx.getGlobal<Rectangle>("rect").height);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

/*
 * main.cpp -- test JavaScript Line
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

#include <malikania/Line.h>

using namespace malikania;

TEST(LineTest, simple)
{
	js::Context ctx;

	ctx.push(Line{-10, -20, 10, 20});

	ASSERT_EQ(-10, ctx.get<Line>(-1).x1);
	ASSERT_EQ(-20, ctx.get<Line>(-1).y1);
	ASSERT_EQ( 10, ctx.get<Line>(-1).x2);
	ASSERT_EQ( 20, ctx.get<Line>(-1).y2);
}

TEST(LineTest, fromScriptFull)
{
	js::Context ctx;

	ctx.evalString("line = { x1: -10, y1: -20, x2: 10, y2: 20 }");

	ASSERT_EQ(-10, ctx.getGlobal<Line>("line").x1);
	ASSERT_EQ(-20, ctx.getGlobal<Line>("line").y1);
	ASSERT_EQ( 10, ctx.getGlobal<Line>("line").x2);
	ASSERT_EQ( 20, ctx.getGlobal<Line>("line").y2);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

/*
 * main.cpp -- test JavaScript Color
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

#include <malikania/Color.h>

using namespace malikania;

TEST(ColorTest, simple)
{
	js::Context ctx;

	ctx.push(Color{10, 20, 30, 40});

	ASSERT_EQ(10, ctx.get<Color>(-1).red);
	ASSERT_EQ(20, ctx.get<Color>(-1).green);
	ASSERT_EQ(30, ctx.get<Color>(-1).blue);
	ASSERT_EQ(40, ctx.get<Color>(-1).alpha);
}

TEST(ColorTest, fromScriptFull)
{
	js::Context ctx;

	ctx.evalString("color = { red: 10, green: 20, blue: 30, alpha: 40 }");

	ASSERT_EQ(10, ctx.getGlobal<Color>("color").red);
	ASSERT_EQ(20, ctx.getGlobal<Color>("color").green);
	ASSERT_EQ(30, ctx.getGlobal<Color>("color").blue);
	ASSERT_EQ(40, ctx.getGlobal<Color>("color").alpha);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}


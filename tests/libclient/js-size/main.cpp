/*
 * main.cpp -- test JavaScript Size
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

#include <malikania/Size.h>

using namespace malikania;

TEST(SizeTest, simple)
{
	js::Context ctx;

	ctx.push(Size{200, 100});

	ASSERT_EQ(200, ctx.get<Size>(-1).width);
	ASSERT_EQ(100, ctx.get<Size>(-1).height);
}

TEST(SizeTest, fromScriptFull)
{
	js::Context ctx;

	ctx.evalString("size = { width: 640, height: 480 };");

	ASSERT_EQ(640, ctx.getGlobal<Size>("size").width);
	ASSERT_EQ(480, ctx.getGlobal<Size>("size").height);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

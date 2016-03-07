/*
 * main.cpp -- test Rectangle
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

#include <gtest/gtest.h>

#include <malikania/Rectangle.h>

using namespace malikania;

TEST(Basics, none)
{
	Rectangle rectangle;

	ASSERT_EQ(0, rectangle.x());
	ASSERT_EQ(0, rectangle.y());
	ASSERT_EQ(0U, rectangle.width());
	ASSERT_EQ(0U, rectangle.height());
}

TEST(Basics, null)
{
	ASSERT_TRUE(Rectangle().isNull());
	ASSERT_FALSE(Rectangle(0, 0, 10, 0).isNull());
	ASSERT_FALSE(Rectangle(0, 0, 0, 10).isNull());
	ASSERT_FALSE(Rectangle(0, 0, 10, 10).isNull());
}

TEST(Basics, standard)
{
	Rectangle rectangle(10, 20, 30, 40);

	ASSERT_EQ(10, rectangle.x());
	ASSERT_EQ(20, rectangle.y());
	ASSERT_EQ(30U, rectangle.width());
	ASSERT_EQ(40U, rectangle.height());
}

TEST(Basics, operatorEq)
{
	Rectangle rectangle1, rectangle2;

	ASSERT_EQ(rectangle1, rectangle2);
}

TEST(Basics, operatorEq1)
{
	Rectangle rectangle1(10, 20, 30, 40);
	Rectangle rectangle2(10, 20, 30, 40);

	ASSERT_EQ(rectangle1, rectangle2);
}

TEST(Basics, operatorNeq)
{
	ASSERT_NE(Rectangle(10), Rectangle(20));
	ASSERT_NE(Rectangle(10, 10), Rectangle(10, 20));
	ASSERT_NE(Rectangle(10, 10, 10), Rectangle(10, 10, 20));
	ASSERT_NE(Rectangle(10, 10, 10, 10), Rectangle(10, 10, 10, 20));
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

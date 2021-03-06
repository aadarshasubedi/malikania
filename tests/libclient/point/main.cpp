/*
 * main.cpp -- test Point
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

#include <malikania/Point.h>

using namespace malikania;

TEST(Basics, none)
{
	Point point;

	ASSERT_EQ(0, point.x());
	ASSERT_EQ(0, point.y());
}

TEST(Basics, standard)
{
	Point point(10, 20);

	ASSERT_EQ(10, point.x());
	ASSERT_EQ(20, point.y());
}

TEST(Basics, operatorEq)
{
	Point point1, point2;

	ASSERT_EQ(point1, point2);
}

TEST(Basics, operatorEq1)
{
	Point point1(10, 20);
	Point point2(10, 20);

	ASSERT_EQ(point1, point2);
}

TEST(Basics, operatorNeq)
{
	ASSERT_NE(Point(10), Point(20));
	ASSERT_NE(Point(10, 10), Point(10, 20));
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

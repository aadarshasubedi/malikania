/*
 * main.cpp -- test Line
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

#include <malikania/Line.h>

using namespace malikania;

TEST(Basics, none)
{
	Line line;

	ASSERT_EQ(0, line.x1());
	ASSERT_EQ(0, line.y1());
	ASSERT_EQ(0, line.x2());
	ASSERT_EQ(0, line.y2());
}

TEST(Basics, standard)
{
	Line line(10, 20, 30, 40);

	ASSERT_EQ(10, line.x1());
	ASSERT_EQ(20, line.y1());
	ASSERT_EQ(30, line.x2());
	ASSERT_EQ(40, line.y2());
}

TEST(Basics, operatorEq)
{
	Line line1, line2;

	ASSERT_EQ(line1, line2);
}

TEST(Basics, operatorEq1)
{
	Line line1(10, 20, 30, 40);
	Line line2(10, 20, 30, 40);

	ASSERT_EQ(line1, line2);
}

TEST(Basics, operatorNeq)
{
	ASSERT_NE(Line(10), Line(20));
	ASSERT_NE(Line(10, 10), Line(10, 20));
	ASSERT_NE(Line(10, 10, 10), Line(10, 10, 20));
	ASSERT_NE(Line(10, 10, 10, 10), Line(10, 10, 10, 20));
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

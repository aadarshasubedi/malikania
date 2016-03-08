/*
 * main.cpp -- test Size
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

#include <malikania/Size.h>

using namespace malikania;

TEST(Basics, none)
{
	Size size;

	ASSERT_EQ(0U, size.width());
	ASSERT_EQ(0U, size.height());
}

TEST(Basics, null)
{
	ASSERT_TRUE(Size().isNull());
	ASSERT_FALSE(Size(0, 10).isNull());
	ASSERT_FALSE(Size(10, 0).isNull());
}

TEST(Basics, standard)
{
	Size size(10, 20);

	ASSERT_EQ(10U, size.width());
	ASSERT_EQ(20U, size.height());
}

TEST(Basics, operatorEq)
{
	Size size1, size2;

	ASSERT_EQ(size1, size2);
}

TEST(Basics, operatorEq1)
{
	Size size1(10, 20);
	Size size2(10, 20);

	ASSERT_EQ(size1, size2);
}

TEST(Basics, operatorNeq)
{
	ASSERT_NE(Size(10), Size(20));
	ASSERT_NE(Size(10, 10), Size(10, 20));
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

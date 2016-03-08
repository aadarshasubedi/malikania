/*
 * main.cpp -- test Color
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

#include <malikania/Color.h>

using namespace malikania;

/*
 * Separate arguments
 * ------------------------------------------------------------------
 */

TEST(Basic, black)
{
	Color c;

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Basic, white)
{
	Color c{255, 255, 255, 255};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Basic, red)
{
	Color c{255, 0, 0, 255};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Basic, green)
{
	Color c{0, 255, 0, 255};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Basic, blue)
{
	Color c{0, 0, 255, 255};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

/*
 * Hexadecimal
 * ------------------------------------------------------------------
 */

TEST(Hex, black)
{
	Color c{0xff000000};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Hex, white)
{
	Color c{0xffffffff};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Hex, red)
{
	Color c{0xffff0000};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Hex, green)
{
	Color c{0xff00ff00};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Hex, blue)
{
	Color c{0xff0000ff};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

/*
 * Names
 * ------------------------------------------------------------------
 *
 * Test most famous.
 */

TEST(Named, black)
{
	Color c{"black"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Named, white)
{
	Color c{"white"};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Named, red)
{
	Color c{"red"};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Named, green)
{
	Color c{"green"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(128, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Named, blue)
{
	Color c{"blue"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(Named, wrong)
{
	try {
		Color c{"does not exist"};

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

/*
 * #rrggbb
 * ------------------------------------------------------------------
 *
 * Test most famous.
 */

TEST(FullRgb, black)
{
	Color c{"#000000"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(FullRgb, white)
{
	Color c{"#ffffff"};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(FullRgb, red)
{
	Color c{"#ff0000"};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(FullRgb, green)
{
	Color c{"#00ff00"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(FullRgb, blue)
{
	Color c{"#0000ff"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(FullRgb, wrong)
{
	try {
		Color c{"#ghijkl"};

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

/*
 * #rgb
 * ------------------------------------------------------------------
 *
 * Test most famous.
 */

TEST(ShortRgb, black)
{
	Color c{"#000"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(ShortRgb, white)
{
	Color c{"#fff"};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(ShortRgb, red)
{
	Color c{"#f00"};

	ASSERT_EQ(255, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(ShortRgb, green)
{
	Color c{"#0f0"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(255, c.green());
	ASSERT_EQ(0, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(ShortRgb, blue)
{
	Color c{"#00f"};

	ASSERT_EQ(0, c.red());
	ASSERT_EQ(0, c.green());
	ASSERT_EQ(255, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(ShortRgb, combination)
{
	Color c{"#123"};

	ASSERT_EQ(17, c.red());
	ASSERT_EQ(34, c.green());
	ASSERT_EQ(51, c.blue());
	ASSERT_EQ(255, c.alpha());
}

TEST(ShortRgb, wrong)
{
	try {
		Color c{"#ghi"};

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
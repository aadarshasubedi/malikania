/*
 * main.cpp -- test Id
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

#include <thread>

#include <gtest/gtest.h>

#include <malikania/Id.h>

using namespace malikania;

class TestId : public testing::Test {
public:
	~TestId()
	{
		Id::reset();
	}
};

TEST_F(TestId, simple)
{
	ASSERT_EQ(0U, Id::next());
	ASSERT_EQ(1U, Id::next());
	ASSERT_EQ(2U, Id::next());
	ASSERT_EQ(3U, Id::next());
	ASSERT_EQ(4U, Id::next());
}

TEST_F(TestId, reset)
{
	Id::next();
	Id::next();
	Id::next();

	Id::reset();

	ASSERT_EQ(0U, Id::next());
}

TEST_F(TestId, release1)
{
	Id::next();	// 0
	Id::next();	// 1
	Id::next();	// 2
	Id::release(1);

	/*
	 * 0 and 2 are currently in use.
	 *
	 * The next id must be 1 and then 3.
	 */
	ASSERT_EQ(1U, Id::next());
	ASSERT_EQ(3U, Id::next());
}
TEST_F(TestId, release2)
{
	Id::next();	// 0
	Id::next();	// 1
	Id::next();	// 2
	Id::release(1);
	Id::release(0);

	/*
	 * Only 2 is in use, next id must be:
	 *
	 * - 1
	 * - 0
	 * - 3
	 */
	ASSERT_EQ(1U, Id::next());
	ASSERT_EQ(0U, Id::next());
	ASSERT_EQ(3U, Id::next());
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

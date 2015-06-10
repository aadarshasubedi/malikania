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

#include <cstdint>
#include <thread>

#include <gtest/gtest.h>

#include <malikania/Id.h>

using namespace malikania;

/* --------------------------------------------------------
 * Basic use case
 * -------------------------------------------------------- */

class TestId : public testing::Test {
protected:
	IdGen<unsigned> m_idgen;

public:
	~TestId()
	{
		m_idgen.reset();
	}
};

TEST_F(TestId, simple)
{
	ASSERT_EQ(0U, m_idgen.next());
	ASSERT_EQ(1U, m_idgen.next());
	ASSERT_EQ(2U, m_idgen.next());
	ASSERT_EQ(3U, m_idgen.next());
	ASSERT_EQ(4U, m_idgen.next());
}

TEST_F(TestId, reset)
{
	m_idgen.next();
	m_idgen.next();
	m_idgen.next();

	m_idgen.reset();

	ASSERT_EQ(0U, m_idgen.next());
}

TEST_F(TestId, release1)
{
	m_idgen.next();	// 0
	m_idgen.next();	// 1
	m_idgen.next();	// 2
	m_idgen.release(1);

	/*
	 * 0 and 2 are currently in use.
	 *
	 * The next id must be 1 and then 3.
	 */
	ASSERT_EQ(1U, m_idgen.next());
	ASSERT_EQ(3U, m_idgen.next());
}

TEST_F(TestId, release2)
{
	m_idgen.next();	// 0
	m_idgen.next();	// 1
	m_idgen.next();	// 2
	m_idgen.release(1);
	m_idgen.release(0);

	/*
	 * Only 2 is in use, next id must be:
	 *
	 * - 1
	 * - 0
	 * - 3
	 */
	ASSERT_EQ(1U, m_idgen.next());
	ASSERT_EQ(0U, m_idgen.next());
	ASSERT_EQ(3U, m_idgen.next());
}

/* --------------------------------------------------------
 * Id RAII class
 * -------------------------------------------------------- */

TEST(IdLocker, basic)
{
	IdGen<int8_t> gen;
	Id<int8_t> id(gen);

	ASSERT_EQ(0, id);
}

TEST(IdLocker, two)
{
	IdGen<int8_t> gen;
	Id<int8_t> id(gen);
	Id<int8_t> id2(gen);

	ASSERT_EQ(0, id);
	ASSERT_EQ(1, id2);
}

TEST(IdLocker, already)
{
	IdGen<int8_t> gen;
	Id<int8_t> id(gen, gen.next());

	ASSERT_EQ(0, id);
}

/* --------------------------------------------------------
 * Limit test
 * -------------------------------------------------------- */

TEST(Limits, max)
{
	IdGen<int8_t> idgen;
	int8_t last;

	try {
		for (int i = 0; i < 127; ++i) {
			last = idgen.next();
		}
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}

	ASSERT_EQ(126, last);
}

TEST(Limits, fail)
{
	IdGen<int8_t> idgen;
	int8_t last;

	try {
		for (int i = 0; i < 200; ++i) {
			last = idgen.next();
		}

		FAIL() << "Exception expected";
	} catch (const std::exception &ex) {
	}

	ASSERT_EQ(126, last);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

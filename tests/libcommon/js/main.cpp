/*
 * main.cpp -- test Js
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

#include <malikania/Js.h>

using namespace malikania;
using namespace std::string_literals;

class TestJs : public testing::Test {
protected:
	Js m_js;
};

/* --------------------------------------------------------
 * push
 * -------------------------------------------------------- */

TEST_F(TestJs, pushBoolean)
{
	ASSERT_EQ(1, dukx_push(m_js, true));
	ASSERT_EQ(DUK_TYPE_BOOLEAN, duk_get_type(m_js, -1));
	ASSERT_TRUE(duk_get_boolean(m_js, -1));
}

TEST_F(TestJs, pushInt)
{
	ASSERT_EQ(1, dukx_push(m_js, 123));
	ASSERT_EQ(DUK_TYPE_NUMBER, duk_get_type(m_js, -1));
	ASSERT_EQ(123, duk_to_int(m_js, -1));
}

TEST_F(TestJs, pushString1)
{
	ASSERT_EQ(1, dukx_push(m_js, "hello world"));
	ASSERT_EQ(DUK_TYPE_STRING, duk_get_type(m_js, -1));
	ASSERT_STREQ("hello world", duk_get_string(m_js, -1));
}

TEST_F(TestJs, pushString2)
{
	ASSERT_EQ(1, dukx_push(m_js, "hello\0world\0!"s));
	ASSERT_EQ(DUK_TYPE_STRING, duk_get_type(m_js, -1));

	std::string expected{"hello\0world\0!"};

	size_t size;
	const char *str = duk_get_lstring(m_js, -1, &size);
	std::string result{str, size};

	// TODO: fix this
	//ASSERT_TRUE(expected == result);
}

TEST_F(TestJs, pushReal)
{
	ASSERT_EQ(1, dukx_push(m_js, 8.0));
	ASSERT_EQ(DUK_TYPE_NUMBER, duk_get_type(m_js, -1));
	ASSERT_EQ(8.0, duk_to_number(m_js, -1));
}

TEST_F(TestJs, pushNull)
{
	ASSERT_EQ(1, dukx_push(m_js, nullptr));
	ASSERT_EQ(DUK_TYPE_NULL, duk_get_type(m_js, -1));
}

TEST_F(TestJs, pushUndefined)
{
	ASSERT_EQ(1, dukx_push(m_js));
	ASSERT_EQ(DUK_TYPE_UNDEFINED, duk_get_type(m_js, -1));
}

/* --------------------------------------------------------
 * get
 * -------------------------------------------------------- */

TEST_F(TestJs, getBoolean)
{
	dukx_push(m_js, true);
	ASSERT_TRUE(dukx_get<bool>(m_js, -1));
}

TEST_F(TestJs, getInt)
{
	dukx_push(m_js, 123);
	ASSERT_EQ(123, dukx_get<int>(m_js, -1));
}

TEST_F(TestJs, getString)
{
	dukx_push(m_js, "hello world");
	ASSERT_EQ("hello world", dukx_get<std::string>(m_js, -1));
}

TEST_F(TestJs, getReal)
{
	dukx_push(m_js, 8.0);
	ASSERT_EQ(8.0, dukx_get<double>(m_js, -1));
}

/* --------------------------------------------------------
 * require
 * -------------------------------------------------------- */

TEST_F(TestJs, requireBoolean)
{
	dukx_push(m_js, true);
	ASSERT_TRUE(dukx_require<bool>(m_js, -1));
}

TEST_F(TestJs, requireInt)
{
	dukx_push(m_js, 123);
	ASSERT_EQ(123, dukx_require<int>(m_js, -1));
}

TEST_F(TestJs, requireString)
{
	dukx_push(m_js, "hello world");
	ASSERT_EQ("hello world", dukx_require<std::string>(m_js, -1));
}

TEST_F(TestJs, requireReal)
{
	dukx_push(m_js, 8.0);
	ASSERT_EQ(8.0, dukx_require<double>(m_js, -1));
}

/* --------------------------------------------------------
 * ref
 * -------------------------------------------------------- */

TEST_F(TestJs, refSimple)
{
	// Push
	dukx_push(m_js, 123);
	auto top = duk_get_top(m_js);
	auto ref = dukx_ref(m_js);
	ASSERT_EQ(0, ref);
	ASSERT_EQ(top - 1, duk_get_top(m_js));

	// Get
	dukx_refget(m_js, ref);
	ASSERT_EQ(123, duk_to_int(m_js, -1));
}

TEST_F(TestJs, refDouble)
{
	// Push one
	dukx_push(m_js, 1);
	int ref1 = dukx_ref(m_js);

	// Push second
	dukx_push(m_js, 2);
	int ref2 = dukx_ref(m_js);

	ASSERT_EQ(0, ref1);
	ASSERT_EQ(1, ref2);

	// Now get back
	dukx_refget(m_js, ref1);
	ASSERT_EQ(1, duk_to_int(m_js, -1));
	dukx_refget(m_js, ref2);
	ASSERT_EQ(2, duk_to_int(m_js, -1));
}

TEST_F(TestJs, refMiddle)
{
	/*
	 * Push 1.
	 * Push 2.
	 * Remove 2
	 * Push 3.
	 *
	 * Normally, 3 will have reference 1 (the second value).
	 */
	dukx_push(m_js, 1);
	int ref1 = dukx_ref(m_js);

	dukx_push(m_js, 2);
	int ref2 = dukx_ref(m_js);

	dukx_unref(m_js, ref2);
	dukx_push(m_js, 3);
	int ref3 = dukx_ref(m_js);

	ASSERT_EQ(0, ref1);
	ASSERT_EQ(1, ref2);
	ASSERT_EQ(1, ref3);

	// Be sure values are correct
	dukx_refget(m_js, ref1);
	ASSERT_EQ(1, duk_to_int(m_js, -1));

	dukx_refget(m_js, ref3);
	ASSERT_EQ(3, duk_to_int(m_js, -1));
}

TEST_F(TestJs, jsRef)
{
	{
		dukx_push(m_js, 123);
		JsRef ref{m_js, dukx_ref(m_js)};

		dukx_refget(m_js, ref);

		ASSERT_EQ(0, static_cast<int>(ref));
		ASSERT_EQ(DUK_TYPE_NUMBER, duk_get_type(m_js, -1));
		ASSERT_EQ(123, duk_to_int(m_js, -1));
	}

	// Here does not exists
	dukx_refget(m_js, 0);

	ASSERT_EQ(DUK_TYPE_UNDEFINED, duk_get_type(m_js, -1));
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

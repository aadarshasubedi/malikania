/*
 * main.cpp -- test util
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

#include <malikania/Util.h>

using namespace malikania;

TEST(Basic, simple)
{
	std::string input = "hello world\r\n\r\n";
	std::vector<std::string> messages = util::netsplit(input);

	ASSERT_EQ(1U, messages.size());
	ASSERT_EQ("hello world", messages[0]);
	ASSERT_TRUE(input.empty());
}

TEST(Basic, two)
{
	std::string input = "hello world\r\n\r\nhow are you?\r\n\r\n";
	std::vector<std::string> messages = util::netsplit(input);

	ASSERT_EQ(2U, messages.size());
	ASSERT_EQ("hello world", messages[0]);
	ASSERT_EQ("how are you?", messages[1]);
	ASSERT_TRUE(input.empty());
}

TEST(Basic, imcomplete)
{
	std::string input = "hello world\r\n";
	std::vector<std::string> messages = util::netsplit(input);

	ASSERT_EQ(0U, messages.size());
	ASSERT_EQ("hello world\r\n", input);
}

TEST(Basic, empty)
{
	std::string input = "hello world\r\n\r\n\r\n\r\nhow are you?\r\n\r\n";
	std::vector<std::string> messages = util::netsplit(input);

	ASSERT_EQ(3U, messages.size());
	ASSERT_EQ("hello world", messages[0]);
	ASSERT_TRUE(messages[1].empty());
	ASSERT_EQ("how are you?", messages[2]);
	ASSERT_TRUE(input.empty());
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

/*
 * main.cpp -- test ElapsedTimer
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

#include <malikania/ElapsedTimer.h>

using namespace malikania;
using namespace std::chrono_literals;

/*
 * For all tests, we tolerate 3ms
 */

class TestElapsedTimer : public testing::Test {
protected:
	ElapsedTimer m_timer;

	inline void assertRange(int value, int expected) const noexcept
	{
		if (value < (expected - 3) || value > (expected + 3)) {
			FAIL() << value << " is bigger than [" << (expected - 3) << ", " << (expected + 3) << "]";
		}
	}
};

TEST_F(TestElapsedTimer, standard)
{
	std::this_thread::sleep_for(300ms);

	assertRange(m_timer.elapsed(), 300);
}

TEST_F(TestElapsedTimer, reset)
{
	std::this_thread::sleep_for(300ms);

	m_timer.reset();

	assertRange(m_timer.elapsed(), 0);
}

TEST_F(TestElapsedTimer, pause)
{
	/*
	 * Simulate a pause in the game like this:
	 *
	 * start     pause restart elapsed
	 * |   10ms   |.5ms.| 6ms  |
	 *
	 * Since the game was paused, the 5ms must not be totalized.
	 */
	std::this_thread::sleep_for(10ms);

	m_timer.pause();

	std::this_thread::sleep_for(5ms);

	m_timer.restart();

	std::this_thread::sleep_for(6ms);

	assertRange(m_timer.elapsed(), 16);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

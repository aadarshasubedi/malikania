/*
 * main.cpp -- test Animation
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

#include <chrono>
#include <thread>
#include <exception>

#include <gtest/gtest.h>

#include <malikania/Animation.h>
#include <malikania/Animator.h>
#include <malikania/ClientResourcesLoader.h>
#include <malikania/ResourcesLocator.h>
#include <malikania/Window.h>

using namespace malikania;

using namespace std::chrono_literals;

namespace {

Window window(400, 400);

} // !namespace

class TestAnimation : public testing::Test {
protected:
	ResourcesLocatorDirectory m_locator;
	ClientResourcesLoader m_loader;

public:
	TestAnimation()
		: m_locator(SOURCE_DIRECTORY "/resources")
		, m_loader(window, m_locator)
	{
	}
};

TEST_F(TestAnimation, standard)
{
	try {
		Animation animation = m_loader.loadAnimation("animations/margins.json");

		ASSERT_EQ(12U, animation.frames().size());
		ASSERT_EQ(500U, animation[0].delay());
		ASSERT_EQ(501U, animation[1].delay());
		ASSERT_EQ(502U, animation[2].delay());
		ASSERT_EQ(503U, animation[3].delay());
		ASSERT_EQ(504U, animation[4].delay());
		ASSERT_EQ(505U, animation[5].delay());
		ASSERT_EQ(506U, animation[6].delay());
		ASSERT_EQ(507U, animation[7].delay());
		ASSERT_EQ(508U, animation[8].delay());
		ASSERT_EQ(509U, animation[9].delay());
		ASSERT_EQ(510U, animation[10].delay());
		ASSERT_EQ(511U, animation[11].delay());
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

TEST_F(TestAnimation, draw)
{
	ElapsedTimer timer;

	try {
		Animation animation = m_loader.loadAnimation("animations/margins.json");
		Animator animator(std::make_shared<Animation>(animation));

		auto x = (400 / 2) - (animation.sprite()->cell().width() / 2);
		auto y = (400 / 2) - (animation.sprite()->cell().height() / 2);

		while (timer.elapsed() < 8000) {
			window.clear();
			animator.draw(window, Point(x, y));
			animator.update();
			window.present();
		}
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

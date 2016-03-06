/*
 * main.cpp -- test Image
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

#include <gtest/gtest.h>

#include <malikania/ClientResourcesLoader.h>
#include <malikania/Image.h>
#include <malikania/ResourcesLocator.h>
#include <malikania/Window.h>

using namespace malikania;

using namespace std::chrono_literals;

namespace {

Window window(52, 52);

} // !namespace

class TestImage : public testing::Test {
protected:
	ResourcesLocatorDirectory m_locator;
	ClientResourcesLoader m_loader;

public:
	TestImage()
		: m_locator(SOURCE_DIRECTORY "/resources")
		, m_loader(window, m_locator)
	{
	}
};

TEST_F(TestImage, image100x10)
{
	try {
		Image image = m_loader.loadImage("images/100x10.png");

		ASSERT_EQ(100U, image.size().width());
		ASSERT_EQ(10U, image.size().height());
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

TEST_F(TestImage, image16x16)
{
	try {
		Image image = m_loader.loadImage("images/16x16.png");

		ASSERT_EQ(16U, image.size().width());
		ASSERT_EQ(16U, image.size().height());
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

TEST_F(TestImage, image10x100)
{
	try {
		Image image = m_loader.loadImage("images/10x100.png");

		ASSERT_EQ(10U, image.size().width());
		ASSERT_EQ(100U, image.size().height());
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

TEST_F(TestImage, notfound)
{
	try {
		m_loader.loadImage("image-not-found");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

/*
 * Draw
 * ------------------------------------------------------------------
 */

TEST_F(TestImage, draw)
{
	try {
		Image image = m_loader.loadImage("images/smiley.png");

		window.clear();
		image.draw(window, Point(10, 10));
		window.present();

		std::this_thread::sleep_for(3s);
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

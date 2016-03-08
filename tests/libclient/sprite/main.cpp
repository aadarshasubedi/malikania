/*
 * main.cpp -- test Sprite
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

#include <malikania/ClientResourcesLoader.h>
#include <malikania/ResourcesLocator.h>
#include <malikania/Sprite.h>
#include <malikania/Window.h>

using namespace malikania;

using namespace std::chrono_literals;

namespace {

Window window;

} // !namespace

class TestSprite : public testing::Test {
protected:
	ResourcesLocatorDirectory m_locator;
	ClientResourcesLoader m_loader;

public:
	TestSprite()
		: m_locator(SOURCE_DIRECTORY "/resources")
		, m_loader(window, m_locator)
	{
	}
};

/*
 * Missing properties
 * ------------------------------------------------------------------
 */

TEST_F(TestSprite, missingPropertyImage)
{
	try {
		m_loader.loadSprite("sprites/no-property-image.json");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

TEST_F(TestSprite, missingPropertyCell)
{
	try {
		m_loader.loadSprite("sprites/no-property-cell.json");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

/*
 * Invalid properties
 * ------------------------------------------------------------------
 */

TEST_F(TestSprite, imageNotString)
{
	try {
		m_loader.loadSprite("sprites/property-image-not-string.json");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

TEST_F(TestSprite, cellNotArray)
{
	try {
		m_loader.loadSprite("sprites/property-cell-not-array.json");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

TEST_F(TestSprite, cellNotArray2)
{
	try {
		m_loader.loadSprite("sprites/property-cell-not-array2.json");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

/*
 * Other errors
 * ------------------------------------------------------------------
 */

TEST_F(TestSprite, imageNotFound)
{
	try {
		m_loader.loadSprite("sprites/image-not-found.json");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

TEST_F(TestSprite, notObject)
{
	try {
		m_loader.loadSprite("sprites/not-object.json");

		FAIL() << "exception expected";
	} catch (const std::exception &) {
	}
}

/*
 * Valid sprites
 * ------------------------------------------------------------------
 */

TEST_F(TestSprite, standard)
{
	try {
		Sprite sprite = m_loader.loadSprite("sprites/simple.json");

		ASSERT_EQ(300U, sprite.cell().width());
		ASSERT_EQ(300U, sprite.cell().height());
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

TEST_F(TestSprite, margins)
{
	try {
		Sprite sprite = m_loader.loadSprite("sprites/margins.json");

		ASSERT_EQ(3U, sprite.rows());
		ASSERT_EQ(4U, sprite.columns());
	} catch (const std::exception &ex) {
		FAIL() << ex.what();
	}
}

TEST_F(TestSprite, draw)
{
	try {
		Sprite sprite = m_loader.loadSprite("sprites/margins.json");

		unsigned total = sprite.rows() * sprite.columns();

		for (unsigned c = 0; c < total; ++c) {
			window.clear();
			sprite.draw(window, c, Point(10, 10));
			window.present();

			std::this_thread::sleep_for(1s);
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

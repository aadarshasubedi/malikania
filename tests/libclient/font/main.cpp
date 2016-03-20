/*
 * main.cpp -- test Font
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

#include <malikania/Color.h>
#include <malikania/Font.h>
#include <malikania/ResourcesLocator.h>
#include <malikania/Window.h>

using namespace malikania;

using namespace std::chrono_literals;

namespace {

Window window(400, 400);

} // !namespace

class TestFont : public testing::Test {
protected:
	ResourcesLocatorDirectory m_locator;
	Font m_font;

public:
	TestFont()
		: m_locator(SOURCE_DIRECTORY "/resources")
		, m_font(m_locator.read("DejaVuSans.ttf"), 10)
	{
		window.setDrawingColor(Color("black"));
		window.clear();
	}
};

TEST_F(TestFont, topleft)
{
	window.setDrawingColor(Color("white"));
	window.drawText("top left", m_font, Point(10, 10));
	window.present();

	std::this_thread::sleep_for(1s);
}

TEST_F(TestFont, topright)
{
	Size dim = m_font.clip("top right");

	window.setDrawingColor(Color("white"));
	window.drawText("top right", m_font, Point(400 - dim.width() - 10, 10));
	window.present();

	std::this_thread::sleep_for(1s);
}


TEST_F(TestFont, bottomleft)
{
	Size dim = m_font.clip("bottom left");

	window.setDrawingColor(Color("white"));
	window.drawText("bottom left", m_font, Point(10, 400 - dim.height() - 10));
	window.present();

	std::this_thread::sleep_for(1s);
}

TEST_F(TestFont, bottomright)
{
	Size dim = m_font.clip("bottom right");

	window.setDrawingColor(Color("white"));
	window.drawText("bottom right", m_font, Point(400 - dim.width() - 10, 400 - dim.height() - 10));
	window.present();

	std::this_thread::sleep_for(1s);
}

TEST_F(TestFont, center)
{
	Size dim = m_font.clip("center");

	window.setDrawingColor(Color("white"));
	window.drawText("center", m_font, Point(200 - (dim.width() / 2), 200 - (dim.height() -2)));
	window.present();

	std::this_thread::sleep_for(1s);
}

TEST_F(TestFont, center2)
{
	Size dim = m_font.clip("The world is Malikania.");

	window.setDrawingColor(Color("white"));
	window.drawText("The world is Malikania.", m_font, Point(200 - (dim.width() / 2), 200 - (dim.height() -2)));
	window.present();

	std::this_thread::sleep_for(3s);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

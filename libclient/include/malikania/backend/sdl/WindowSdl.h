/*
 * WindowSdl.h -- window object (SDL2 implementation)
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

#ifndef _MALIKANIA_WINDOW_SDL_H_
#define _MALIKANIA_WINDOW_SDL_H_

#include <memory>
#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <malikania/Size.h>
#include <malikania/Color.h>
#include <malikania/Line.h>
#include <malikania/Point.h>
#include <malikania/Rectangle.h>
#include <malikania/Font.h>

namespace malikania {

class Window;

class WindowSdl {
private:
	using WindowHandle = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>;
	using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;

	WindowHandle m_window;
	RendererHandle m_renderer;

public:
	WindowSdl(unsigned width, unsigned height);

	void close();
	void processEvents(Window &window);
	void clear();
	void update();
	void present();
	Size resolution();

	// Drawing functions
	void setDrawingColor(const Color &color);
	void drawLine(const Line &line);
	void drawLines(const std::vector<Point> &points);
	void drawPoint(const Point &point);
	void drawPoints(const std::vector<Point> &points);
	void drawRectangle(const Rectangle &rectangle, bool filled, const Color &fillColor);
	void drawRectangles(const std::vector<Rectangle> &rectangles, bool filled, std::vector<Color> fillColors);
	void drawText(const std::string &text, Font &font, const Rectangle &rectangle);
	void drawText(const std::string &text, Font &font, const Point &point);

	inline SDL_Renderer *renderer() noexcept
	{
		return m_renderer.get();
	}
};

/*
 * "Export" the backend.
 */
using BackendWindow = WindowSdl;

} // !malikania

#endif // !_MALIKANIA_WINDOW_SDL_H_

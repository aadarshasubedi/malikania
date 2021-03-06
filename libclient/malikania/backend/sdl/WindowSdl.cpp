/*
 * WindowSdl.cpp -- window object (SDL2 implementation)
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

#include <stdexcept>

#include <malikania/Window.h>

namespace malikania {

WindowSdl::WindowSdl(unsigned width, unsigned height)
	: m_window(nullptr, nullptr)
	, m_renderer(nullptr, nullptr)
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	m_window = WindowHandle(
		SDL_CreateWindow("Malikania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL),
		SDL_DestroyWindow
	);

	if (m_window == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	// Create renderer
	m_renderer = RendererHandle(
		SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
		SDL_DestroyRenderer
	);

	if (m_renderer == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	if (TTF_Init() == -1) {
		throw std::runtime_error(TTF_GetError());
	}
}

void WindowSdl::processEvents(Window &window)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYUP:
			window.onKeyUp(event.key.keysym.sym);
			break;
		// TODO continue implemanting all event possible
		case SDL_KEYDOWN:
			window.onKeyDown(event.key.keysym.sym);
			break;
		case SDL_QUIT:
			window.close();
			break;
		default:
			break;
		}
	}
}

void WindowSdl::clear()
{
	SDL_RenderClear(m_renderer.get());
}

void WindowSdl::update()
{
	// TO AUDIT
#if 0
	for (Refresh function : m_refreshList) {
		function();
	}
#endif
}

void WindowSdl::present()
{
	SDL_RenderPresent(m_renderer.get());
}

Size WindowSdl::resolution()
{
	SDL_DisplayMode current;
	int width = 0;
	int height = 0;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
		int error = SDL_GetCurrentDisplayMode(i, &current);
		if (error == 0) {
			// Get the last one
			// TODO test with only one display mode, but we have to test with more than that
			width = current.w;
			height = current.h;
		} else {
			throw std::runtime_error("Could not get display mode for video display" + std::string(SDL_GetError()));
		}
	}

	return Size((unsigned)width, (unsigned)height);
}

void WindowSdl::setDrawingColor(const Color &color)
{
	int error = SDL_SetRenderDrawColor(m_renderer.get(), color.red(), color.green(), color.blue(), color.alpha());
	if (error != 0) {
		throw std::runtime_error("Couldn't set drawing color" + std::string(SDL_GetError()));
	}
}

void WindowSdl::drawLine(const Line &line)
{
	if (SDL_RenderDrawLine(m_renderer.get(), line.x1(), line.y1(), line.x2(), line.y2()) != 0) {
		throw std::runtime_error(SDL_GetError());
	}
}

void WindowSdl::drawLines(const std::vector<Point> &points)
{
	std::vector<SDL_Point> sdlPoints(points.size());

	for (unsigned i = 0; i < points.size(); ++i) {
		sdlPoints[i] = SDL_Point{points[i].x(), points[i].y()};
	}

	if (SDL_RenderDrawLines(m_renderer.get(), sdlPoints.data(), sdlPoints.size()) != 0) {
		throw std::runtime_error(SDL_GetError());
	}
}

void WindowSdl::drawPoint(const Point &point)
{
	if (SDL_RenderDrawPoint(m_renderer.get(), point.x(), point.y()) != 0) {
		throw std::runtime_error(SDL_GetError());
	}
}

void WindowSdl::drawPoints(const std::vector<Point> &points)
{
	std::vector<SDL_Point> sdlPoints(points.size());

	for (unsigned i = 0; i < points.size(); ++i) {
		sdlPoints[i] = SDL_Point{points[i].x(), points[i].y()};
	}

	if (SDL_RenderDrawPoints(m_renderer.get(), sdlPoints.data(), sdlPoints.size()) != 0) {
		throw std::runtime_error(SDL_GetError());
	}
}

// TODO: not sure to keep this signature (add fillRect probably)
void WindowSdl::drawRectangle(const Rectangle &rectangle, bool filled, const malikania::Color &fillColor)
{
	SDL_Rect rect{rectangle.x(), rectangle.y(), (int)rectangle.width(), (int)rectangle.height()};
	int error = SDL_RenderDrawRect(m_renderer.get(), &rect);
	if (error != 0) {
		throw std::runtime_error(SDL_GetError());
	}
	if (filled) {
		this->setDrawingColor(fillColor);
		error = SDL_RenderFillRect(m_renderer.get(), &rect);
		if (error != 0) {
			throw std::runtime_error(SDL_GetError());
		}
	}
}

// TODO: same as above
void WindowSdl::drawRectangles(const std::vector<Rectangle> &rectangles, bool filled, std::vector<Color> fillColors)
{
	std::vector<SDL_Rect> sdlRects(rectangles.size());

	for (unsigned i = 0; i < rectangles.size(); ++i) {
		sdlRects[i] = SDL_Rect{rectangles[i].x(), rectangles[i].y(), (int)rectangles[i].width(), (int)rectangles[i].height()};
	}

	if (SDL_RenderDrawRects(m_renderer.get(), sdlRects.data(), sdlRects.size()) != 0) {
		throw std::runtime_error(SDL_GetError());
	}

	if (filled) {
		if (rectangles.size() != fillColors.size()) {
			throw std::runtime_error("Couldn't fill rectangles, rectangles size and fillColors size are not the same");
		}

		int j = 0;
		for (Color fillColor : fillColors) {
			this->setDrawingColor(fillColor);

			if (SDL_RenderFillRect(m_renderer.get(), &sdlRects[j++]) != 0) {
				throw std::runtime_error("Couldn't fill rectangle" + std::string(SDL_GetError()));
			}
		}
	}
}

// TODO: merge this into only one function and test results.
void WindowSdl::drawText(const std::string &text, Font &font, const Rectangle &rectangle)
{
	SDL_Color color = {0, 0, 0, 255};
	SDL_Surface* message = TTF_RenderUTF8_Blended(font.backend().font(), text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer.get(), message);
	SDL_Rect rect{rectangle.x(), rectangle.y(), (int)rectangle.width(), (int)rectangle.height()};

	SDL_RenderCopy(m_renderer.get(), texture, nullptr, &rect);
	SDL_FreeSurface(message);
}

void WindowSdl::drawText(const std::string &text, Font &font, const Point &point)
{
	SDL_Color color = {0, 0, 0, 0};
	SDL_GetRenderDrawColor(m_renderer.get(), &color.r, &color.g, &color.b, &color.a);
	SDL_Surface* message = TTF_RenderUTF8_Blended(font.backend().font(), text.c_str(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer.get(), message);
	SDL_Rect rect{point.x(), point.y(), message->w, message->h};

	SDL_RenderCopy(m_renderer.get(), textTexture, nullptr, &rect);
	SDL_FreeSurface(message);
}

void WindowSdl::close()
{
}

} // !malikania

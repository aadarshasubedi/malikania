#include <stdexcept>

#include "Window.h"

namespace malikania {

WindowSdl::WindowSdl()
	: m_window(nullptr, nullptr)
	, m_renderer(nullptr, nullptr)
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	m_window = WindowHandle(
		SDL_CreateWindow("Malikania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP),
		SDL_DestroyWindow
	);

	// In the event that the window could not be made...
	if (m_window == nullptr) {
		throw std::runtime_error("Couldn't create create window");
	}

	// Create renderer
	m_renderer = RendererHandle(
		SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
		SDL_DestroyRenderer
	);

	if (m_renderer == nullptr) {
		throw std::runtime_error("Couldn't create a renderer");
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

	return Size({width, height});
}

void WindowSdl::setDrawingColor(const Color &color)
{
	int error = SDL_SetRenderDrawColor(m_renderer.get(), color.red, color.green, color.blue, color.alpha);
	if (error != 0) {
		throw std::runtime_error("Couldn't set drawing color" + std::string(SDL_GetError()));
	}
}

void WindowSdl::drawLine(const Line &line)
{
	int error = SDL_RenderDrawLine(m_renderer.get(), line.startX, line.startY, line.endX, line.endY);
	if (error != 0) {
		throw std::runtime_error("Couldn't draw line" + std::string(SDL_GetError()));
	}
}

void WindowSdl::drawLines(const std::vector<Point> &points)
{
	SDL_Point sdlPoints[points.size()];

	int i = 0;
	for (const Point &point : points) {
		sdlPoints[i++] = {point.x, point.y};
	}

	int error = SDL_RenderDrawLines(m_renderer.get(), sdlPoints, points.size());
	if (error != 0) {
		throw std::runtime_error("Couldn't draw lines" + std::string(SDL_GetError()));
	}
}

void WindowSdl::drawPoint(const Point &point)
{
	int error = SDL_RenderDrawPoint(m_renderer.get(), point.x, point.y);
	if (error != 0) {
		throw std::runtime_error("Couldn't draw point" + std::string(SDL_GetError()));
	}
}

void WindowSdl::drawPoints(const std::vector<Point> &points)
{
	SDL_Point sdlPoints[points.size()];

	int i = 0;
	for (const Point &point : points) {
		sdlPoints[i++] = {point.x, point.y};
	}

	int error = SDL_RenderDrawPoints(m_renderer.get(), sdlPoints, points.size());
	if (error != 0) {
		throw std::runtime_error("Couldn't draw points" + std::string(SDL_GetError()));
	}
}

void WindowSdl::drawRectangle(const Rectangle &rectangle, bool filled, Color fillColor)
{
	SDL_Rect rect{rectangle.x, rectangle.y, rectangle.width, rectangle.height};
	int error = SDL_RenderDrawRect(m_renderer.get(), &rect);
	if (error != 0) {
		throw std::runtime_error("Couldn't draw rectangle" + std::string(SDL_GetError()));
	}
	if (filled) {
		this->setDrawingColor(fillColor);
		error = SDL_RenderFillRect(m_renderer.get(), &rect);
		if (error != 0) {
			throw std::runtime_error("Couldn't fill rectangle" + std::string(SDL_GetError()));
		}
	}
}

void WindowSdl::drawRectangles(const std::vector<Rectangle> &rectangles, bool filled, std::vector<Color> fillColors)
{
	SDL_Rect sdlRects[rectangles.size()];

	int i = 0;
	for (const Rectangle &rectangle : rectangles) {
		sdlRects[i++] = {rectangle.x, rectangle.y, rectangle.width, rectangle.height};
	}

	int error = SDL_RenderDrawRects(m_renderer.get(), sdlRects, rectangles.size());
	if (error != 0) {
		throw std::runtime_error("Couldn't draw rectangles" + std::string(SDL_GetError()));
	}

	if (filled) {
		if (rectangles.size() != fillColors.size()) {
			throw std::runtime_error("Couldn't fill rectangles, rectangles size and fillColors size are not the same");
		}
		int j = 0;
		for (Color fillColor : fillColors) {
			this->setDrawingColor(fillColor);
			error = SDL_RenderFillRect(m_renderer.get(), &sdlRects[j++]);
			if (error != 0) {
				throw std::runtime_error("Couldn't fill rectangle" + std::string(SDL_GetError()));
			}
		}
	}
}

void WindowSdl::close()
{
}

} // !malikania

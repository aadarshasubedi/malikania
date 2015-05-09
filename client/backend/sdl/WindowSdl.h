#ifndef _MALIKANIA_WINDOW_SDL_H_
#define _MALIKANIA_WINDOW_SDL_H_

#include <memory>
#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <Size.h>
#include <Color.h>
#include <Line.h>
#include <Point.h>
#include <Rectangle.h>

namespace malikania {

class Window;

class WindowSdl {
private:
	using WindowHandle = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>;
	using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;

	WindowHandle m_window;
	RendererHandle m_renderer;

public:
	WindowSdl();

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
	void drawRectangle(const Rectangle &rectangle, bool filled, Color fillColor);
	void drawRectangles(const std::vector<Rectangle> &rectangles, bool filled, std::vector<Color> fillColors);

	inline SDL_Renderer *renderer() noexcept
	{
		return m_renderer.get();
	}
};

using BackendWindow = WindowSdl;

} // !malikania

#endif // !_MALIKANIA_WINDOW_SDL_H_

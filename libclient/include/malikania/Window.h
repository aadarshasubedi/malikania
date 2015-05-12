#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <vector>
#include <string>
#include <map>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/WindowSdl.h"
#endif

#include "Size.h"
#include "Line.h"
#include "Color.h"
#include "Point.h"

namespace malikania {

class Window {
public:
	using KeyUp = std::function<void (int)>;
	using KeyDown = std::function<void (int)>;
	using MouseMove = std::function<void (int, int)>;
	using Refresh = std::function<void (void)>;
	using KeyUpList = std::vector<KeyUp>;
	using KeyDownList = std::vector<KeyDown>;
	using MouseMoveList = std::vector<MouseMove>;
	using RefreshList = std::vector<Refresh>;

private:
	BackendWindow m_backend;
	bool m_isOpen{true};
	KeyUpList m_keyUpList;
	KeyDownList m_keyDownList;
	MouseMoveList m_mouseMoveList;
	RefreshList m_refreshList;
	Color m_drawingColor;

	template <typename FuncList, typename... Args>
	inline void notify(FuncList list, Args&&... args)
	{
		for (auto &f : list) {
			f(std::forward<Args>(args)...);
		}
	}

public:
	inline BackendWindow &backend() noexcept
	{
		return m_backend;
	}

	inline Size getWindowResolution()
	{
		return m_backend.resolution();
	}

	inline void processEvent()
	{
		m_backend.processEvents(*this);
	}

	inline void clear()
	{
		m_backend.clear();
	}

	inline void update()
	{
		m_backend.update();
	}

	inline void present()
	{
		m_backend.present();
	}

	inline bool isOpen() noexcept
	{
		return m_isOpen;
	}

	inline void close() noexcept
	{
		m_isOpen = false;
		m_backend.close();
	}

	void setOnKeyUp(KeyUp function);
	void setOnKeyDown(KeyDown function);
	void setOnMouseMove(MouseMove function);
	void setOnRefresh(Refresh function);

	inline void onKeyUp(int key)
	{
		notify(m_keyUpList, key);
	}

	inline void onKeyDown(int key)
	{
		notify(m_keyDownList, key);
	}

	inline void onMouseMove(int x, int y)
	{
		notify(m_mouseMoveList, x, y);
	}

	inline Color drawingColor()
	{
		return m_drawingColor;
	}

	inline void setDrawingColor(Color color)
	{
		m_drawingColor = std::move(color);
		m_backend.setDrawingColor(color);
	}

	inline void drawLine(const Line &line)
	{
		m_backend.drawLine(line);
	}

	inline void drawLines(const std::vector<Point> &points)
	{
		m_backend.drawLines(points);
	}

	inline void drawPoint(const Point &point)
	{
		m_backend.drawPoint(point);
	}

	inline void drawPoints(const std::vector<Point> &points)
	{
		m_backend.drawPoints(points);
	}

	inline void drawRectangle(const Rectangle &rectangle, bool filled = false, Color fillColor = {255, 255, 255, 255})
	{
		m_backend.drawRectangle(rectangle, filled, fillColor);
	}

	inline void drawRectangles(const std::vector<Rectangle> &rectangles, bool filled = false
		, std::vector<Color> fillColors = {{255, 255, 255, 255}})
	{
		m_backend.drawRectangles(rectangles, filled, fillColors);
	}
};

}// !malikania

#endif // WINDOW_H

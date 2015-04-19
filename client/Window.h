#ifndef WINDOW_H
#define WINDOW_H
#define SDL_MAIN_HANDLED

#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <SDL.h>
#include "Size.h"

namespace malikania {

// Usefull to handle C pointer
using WindowHandle = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>;
using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;

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
	bool m_isOpen;
	KeyUpList m_keyUpList;
	KeyDownList m_keyDownList;
	MouseMoveList m_mouseMoveList;
	RefreshList m_refreshList;

public:
	Window();
	bool isOpen() noexcept;
	void processEvent();
	void clear();
	void update();
	void draw();
	void close() noexcept;
	static void quit();
	void onKeyUp(KeyUp function);
	void onKeyDown(KeyDown function);
	void onMouseMove(MouseMove function);
	void onRefresh(Refresh function);
	Size getWindowResolution();
	static WindowHandle& window();
	static RendererHandle& renderer();
};

}// !malikania

#endif // WINDOW_H

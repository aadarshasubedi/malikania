#ifndef _MALIKANIA_WINDOW_SDL_H_
#define _MALIKANIA_WINDOW_SDL_H_

#include <memory>

#define SDL_MAIN_HANDLED
#include <SDL.h>

namespace malikania {

class WindowSdl {
private:
	using WindowHandle = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>;
	using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;

	WindowHandle m_window;
	RendererHandle m_renderer;

public:
	WindowSdl();
};

} // !malikania

using BackendWindow = WindowSdl;

#endif // !_MALIKANIA_WINDOW_SDL_H_

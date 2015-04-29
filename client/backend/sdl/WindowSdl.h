#ifndef _MALIKANIA_WINDOW_SDL_H_
#define _MALIKANIA_WINDOW_SDL_H_

#include <memory>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <Size.h>

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

	inline SDL_Renderer *renderer() noexcept
	{
		return m_renderer.get();
	}
};

using BackendWindow = WindowSdl;
using BackendRenderer = SDL_Renderer;

} // !malikania

#endif // !_MALIKANIA_WINDOW_SDL_H_

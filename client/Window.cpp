#include "Window.h"
#include <stdexcept>

namespace malikania {

Window::Window()
	: m_window(nullptr, nullptr), m_isOpen(true)
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	m_window = WindowHandle(
		SDL_CreateWindow("Malikania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL),
		SDL_DestroyWindow
	);

	if (m_window == NULL) {
		// In the event that the window could not be made...
		throw std::runtime_error("Couldn't create create window: ");
	}
}

bool Window::isOpen() noexcept
{
	return m_isOpen;
}

void Window::processEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYUP:
			for (KeyUp function : m_keyUpList) {
				function(event.key.keysym.sym);
			}
			break;
		// TODO continue implemanting all event possible
		case SDL_QUIT:
			m_isOpen = false;
			break;
		default:
			break;
		}
	}
}

void Window::clear()
{
	// TODO
}

void Window::update()
{
	// TODO
}

void Window::draw()
{
	// TODO
}

void Window::close() noexcept
{
	m_isOpen = false;
}

void Window::quit()
{
	SDL_Quit();
}

void Window::onKeyUp(KeyUp function)
{
	m_keyUpList.push_back(std::move(function));
}

void Window::onKeyDown(KeyDown function)
{
	m_keyDownList.push_back(std::move(function));
}

void Window::onMouseMove(MouseMove function)
{
	m_mouseMoveList.push_back(std::move(function));
}

}// !malikania

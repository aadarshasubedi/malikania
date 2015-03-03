#include "Window.h"
#include <stdexcept>
#include <SDL_image.h>

namespace malikania {

Window::Window()
	: m_window(nullptr, nullptr), m_renderer(nullptr, nullptr), m_texture(nullptr, nullptr), m_isOpen(true),
	  m_textureXPosition(0), m_textureYPosition(0)
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	m_window = WindowHandle(
		SDL_CreateWindow("Malikania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL),
		SDL_DestroyWindow
	);

	if (m_window == NULL) {
		// In the event that the window could not be made...
		throw std::runtime_error("Couldn't create create window");
	}

	// Create renderer
#if 1
	m_renderer = RendererHandle(
		SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
		SDL_DestroyRenderer
	);

	if (m_renderer == nullptr) {
		throw std::runtime_error("Couldn't create a renderer");
	}

#endif
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
	SDL_RenderClear(m_renderer.get());
}

void Window::update()
{
	setTexturePosition(m_textureXPosition, m_textureYPosition);
}

void Window::draw()
{
	SDL_RenderPresent(m_renderer.get());
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

void Window::setTexture(std::string imagePath)
{
	// Create Texture
	m_texture = TextureHandle(
		IMG_LoadTexture(m_renderer.get(), imagePath.c_str()),
		SDL_DestroyTexture
	);

	if (m_texture == NULL) {
		std::string error = "Couldn't create a texture: " + std::string(SDL_GetError());
		throw std::runtime_error(error);
	}
}

void Window::updateTexturePosition(int x, int y)
{
	m_textureXPosition = x;
	m_textureYPosition = y;
}

void Window::setTexturePosition(int x, int y)
{
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(m_texture.get(), NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(m_renderer.get(), m_texture.get(), NULL, &pos);
}

}// !malikania

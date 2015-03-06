#include "Window.h"
#include <stdexcept>

namespace malikania {

Window::Window()
	: m_window(nullptr, nullptr), m_renderer(nullptr, nullptr), m_isOpen(true)
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
	for (auto const &pair : m_imageMap) {
		setImagePosition(pair.first, pair.second->getX(), pair.second->getY());
	}
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

void Window::setBackground(ImageHandle image)
{
	// TODO
}

void Window::addImage(std::string id, ImageHandle image)
{
	m_imageMap[id] = std::move(image);
}

void Window::addImage(std::string id, std::string imagePath)
{
	addImage(id, std::make_unique<Image>(Image(imagePath, m_renderer)));
}

Image &Window::getImage(std::string id)
{
	if (m_imageMap.find(id) != m_imageMap.end()) {
		return *m_imageMap[id];
	} else {
		// FIXME Exception or just an error?
		throw std::runtime_error("Image id \"" + id + "\" not found");
	}
}

void Window::updateImagePosition(std::string id, int x, int y)
{
	getImage(id).setPosition(x, y);
}

void Window::setImagePosition(std::string id, int x, int y)
{
	Image& image = getImage(id);
	SDL_Texture* texturePtr = image.getTexture().get();
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(texturePtr, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(m_renderer.get(), texturePtr, NULL, &pos);
}

}// !malikania

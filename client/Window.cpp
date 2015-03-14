#include "Window.h"
#include <stdexcept>
#include "Size.h"

namespace malikania {

Window::Window()
	: m_window(nullptr, nullptr), m_renderer(nullptr, nullptr), m_isOpen(true)
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	m_window = WindowHandle(
		SDL_CreateWindow("Malikania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP),
		SDL_DestroyWindow
	);

	if (m_window == NULL) {
		// In the event that the window could not be made...
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
		case SDL_KEYDOWN:
			for (KeyDown function : m_keyDownList) {
				function(event.key.keysym.sym);
			}
			break;
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
	for (auto const &pair : m_animationMap) {
		Animation& animation = getAnimation(pair.first);
		SDL_Texture* texturePtr = animation.getTexture().get();
		malikania::Rectangle rect(pair.second->x(), pair.second->y(), pair.second->width(), pair.second->height());
		SDL_RenderCopy(m_renderer.get(), texturePtr, pair.second->getRectangle().get(), rect.get());
	}

	for (Refresh function : m_refreshList) {
		function();
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

void Window::onRefresh(Refresh function)
{
	m_refreshList.push_back(std::move(function));
}

void Window::setBackground(ImageHandle image)
{
	// TODO
}

void Window::addAnimation(std::string id, AnimationHandle animation)
{
	m_animationMap[id] = std::move(animation);
}

void Window::addAnimation(std::string id, std::string imagePath, const Size &spriteSize, const Size &cellSize)
{
	addAnimation(id, std::make_unique<Animation>(Animation(imagePath, m_renderer, Rectangle(Position(), spriteSize), cellSize)));
}

Animation &Window::getAnimation(std::string id)
{
	if (m_animationMap.find(id) != m_animationMap.end()) {
		return *m_animationMap[id];
	} else {
		// FIXME Exception or just an error?
		throw std::runtime_error("Image id \"" + id + "\" not found");
	}
}

void Window::updateAnimationPosition(std::string id, int x, int y)
{
	getAnimation(id).setPosition(x, y);
}

void Window::updateAnimationState(std::string id, std::string state)
{
	getAnimation(id).setState(state);
}

void Window::setAnimationCellMap(std::string id, std::map<std::string, Position> cellMap)
{
	getAnimation(id).setCellMap(cellMap);
}

Size Window::getWindowResolution()
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

	return Size(width, height);
}

}// !malikania

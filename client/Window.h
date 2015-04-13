#ifndef WINDOW_H
#define WINDOW_H
#define SDL_MAIN_HANDLED

#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <SDL.h>
#include "Image.h"
#include "Animation.h"
#include "Size.h"

namespace malikania {

// Usefull to handle C pointer
using WindowHandle = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>;
using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;
using ImageHandle = std::unique_ptr<Image>;
using AnimationHandle = std::unique_ptr<Animation>;

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
	WindowHandle m_window;
	bool m_isOpen;
	KeyUpList m_keyUpList;
	KeyDownList m_keyDownList;
	MouseMoveList m_mouseMoveList;
	RefreshList m_refreshList;
	RendererHandle m_renderer;
	ImageHandle m_background;
	std::map<std::string, AnimationHandle> m_animationMap;

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
	void setBackground(ImageHandle image);
	void addAnimation(std::string id, AnimationHandle image);
	void addAnimation(std::string id, std::string imagePath, const Size &spriteSize, const Size &cellSize);
	Animation &getAnimation(std::string id);
	void updateAnimationPosition(std::string id, int x, int y);
	void updateAnimationState(std::string id, std::string state);
	void setAnimationCellMap(std::string id, std::map<std::string, Position> cellMap);
	Size getWindowResolution();
};

}// !malikania

#endif // WINDOW_H

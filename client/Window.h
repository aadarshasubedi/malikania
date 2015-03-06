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

namespace malikania {

// Usefull to handle C pointer
using WindowHandle = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>;
using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;
using ImageHandle = std::unique_ptr<Image>;

class Window {
public:
	using KeyUp = std::function<void (int)>;
	using KeyDown = std::function<void (int)>;
	using MouseMove = std::function<void (int, int)>;
	using KeyUpList = std::vector<KeyUp>;
	using KeyDownList = std::vector<KeyDown>;
	using MouseMoveList = std::vector<MouseMove>;

private:
	WindowHandle m_window;
	bool m_isOpen;
	KeyUpList m_keyUpList;
	KeyDownList m_keyDownList;
	MouseMoveList m_mouseMoveList;
	RendererHandle m_renderer;
	ImageHandle m_background;
	std::map<std::string, ImageHandle> m_imageMap;
	void setImagePosition(std::string id, int x, int y);

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
	void setBackground(ImageHandle image);
	void addImage(std::string id, ImageHandle image);
	void addImage(std::string id, std::string imagePath);
	Image &getImage(std::string id);
	void updateImagePosition(std::string id, int x, int y);
};

}// !malikania

#endif // WINDOW_H

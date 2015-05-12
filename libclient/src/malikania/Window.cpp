#include <stdexcept>

#include "Window.h"

namespace malikania {

void Window::setOnKeyUp(KeyUp function)
{
	m_keyUpList.push_back(std::move(function));
}

void Window::setOnKeyDown(KeyDown function)
{
	m_keyDownList.push_back(std::move(function));
}

void Window::setOnMouseMove(MouseMove function)
{
	m_mouseMoveList.push_back(std::move(function));
}

void Window::setOnRefresh(Refresh function)
{
	m_refreshList.push_back(std::move(function));
}

}// !malikania

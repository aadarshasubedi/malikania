#include <stdexcept>

#include "Window.h"

namespace malikania {

void WindowSdl::onKeyUp(KeyUp function)
{
	m_keyUpList.push_back(std::move(function));
}

void WindowSdl::onKeyDown(KeyDown function)
{
	m_keyDownList.push_back(std::move(function));
}

void WindowSdl::onMouseMove(MouseMove function)
{
	m_mouseMoveList.push_back(std::move(function));
}

void WindowSdl::onRefresh(Refresh function)
{
	m_refreshList.push_back(std::move(function));
}

}// !malikania

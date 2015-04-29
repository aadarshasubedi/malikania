#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <vector>
#include <string>
#include <map>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/WindowSdl.h"
#endif

#include "Size.h"

namespace malikania {

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
	BackendWindow m_backend;
	bool m_isOpen{true};
	KeyUpList m_keyUpList;
	KeyDownList m_keyDownList;
	MouseMoveList m_mouseMoveList;
	RefreshList m_refreshList;

	template <typename FuncList, typename... Args>
	inline void notify(FuncList list, Args&&... args)
	{
		for (auto &f : list) {
			f(std::forward<Args>(args)...);
		}
	}

public:
	inline BackendWindow &backend() noexcept
	{
		return m_backend;
	}

	inline Size getWindowResolution()
	{
		return m_backend.resolution();
	}

	inline BackendRenderer *renderer()
	{
		return m_backend.renderer();
	}

	inline void processEvent()
	{
		m_backend.processEvents(*this);
	}

	inline void clear()
	{
		m_backend.clear();
	}

	inline void update()
	{
		m_backend.update();
	}

	inline void present()
	{
		m_backend.present();
	}

	inline bool isOpen() noexcept
	{
		return m_isOpen;
	}

	inline void close() noexcept
	{
		m_isOpen = false;
		m_backend.close();
	}

	void setOnKeyUp(KeyUp function);
	void setOnKeyDown(KeyDown function);
	void setOnMouseMove(MouseMove function);
	void setOnRefresh(Refresh function);

	inline void onKeyUp(int key)
	{
		notify(m_keyUpList, key);
	}

	inline void onKeyDown(int key)
	{
		notify(m_keyDownList, key);
	}

	inline void onMouseMove(int x, int y)
	{
		notify(m_mouseMoveList, x, y);
	}
};

}// !malikania

#endif // WINDOW_H

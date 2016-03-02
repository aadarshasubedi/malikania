#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <string>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/ImageSdl.h"
#endif

#include <malikania/Size.h>

#include "Point.h"

namespace malikania {

class Window;

class Image {
private:
	BackendImage m_backend;
	Size m_size;

public:
	inline Image(Window &window, const std::string &data)
		: m_backend(*this, window, data)
	{
	}

	inline BackendImage &backend() noexcept
	{
		return m_backend;
	}

	inline void draw(Window &window, const Point &position = {0, 0})
	{
		m_backend.draw(window, position);
	}

	const Size &size() const noexcept;

	void setSize(Size size) noexcept;

};

}// !malikania

#endif // IMAGE_H

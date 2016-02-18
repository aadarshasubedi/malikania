#ifndef _MALIKANIA_IMAGE_H_
#define _MALIKANIA_IMAGE_H_

#include <memory>
#include <string>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/ImageSdl.h"
#endif

#include "Point.h"
#include "Size.h"

namespace malikania {

class Window;

class Image {
private:
	BackendImage m_backend;
	Size m_size;

public:
	inline Image(Window &window, std::string path)
		: m_backend(*this, window, path)
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

#endif // !_MALIKANIA_IMAGE_H_

#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <string>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/ImageSdl.h"
#endif

#include "Rectangle.h"

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

	const Size &size() const noexcept;
	void setSize(Size size) noexcept;

	inline void draw(Window &window, const Position &position = {0, 0})
	{
		m_backend.draw(window, position);
	}
};

}// !malikania

#endif // IMAGE_H

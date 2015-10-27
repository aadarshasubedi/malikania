#ifndef _MALIKANIA_FONT_H_
#define _MALIKANIA_FONT_H_

#include <string>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/FontSdl.h"
#endif

namespace malikania {

class Font {
private:
	BackendFont m_backend;

public:
	inline Font(std::string path, int size)
		: m_backend(path, size)
	{
	}

	inline BackendFont &backend() noexcept
	{
		return m_backend;
	}

	inline const BackendFont &backend() const noexcept
	{
		return m_backend;
	}
};

} // !malikania

#endif // _MALIKANIA_FONT_H_

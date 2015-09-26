#ifndef _MALIKANIA_FONT_SDL_H_
#define _MALIKANIA_FONT_SDL_H_

#include <memory>

#include <SDL.h>

#include "SDL_ttf.h"

namespace malikania {

using FontHandle = std::unique_ptr<TTF_Font, void (*)(TTF_Font*)>;

class FontSdl {
private:
	FontHandle m_font;
	std::string m_path;
	int m_size;

public:
	FontSdl(std::string path, int size);

	inline TTF_Font *font() noexcept
	{
		return m_font.get();
	}
};

/*
 * "Export" the backend.
 */
using BackendFont = FontSdl;

} // !malikania

#endif // !_MALIKANIA_FONT_SDL_H_

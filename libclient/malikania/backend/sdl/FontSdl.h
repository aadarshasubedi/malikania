/*
 * FontSdl.h -- font object (SDL2 implementation)
 *
 * Copyright (c) 2013-2016 Malikania Authors
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _MALIKANIA_FONT_SDL_H_
#define _MALIKANIA_FONT_SDL_H_

#include <memory>

#include <SDL.h>
#include <SDL_ttf.h>

#include <malikania/CommonClient.h>

namespace malikania {

class Font;
class Size;

class MALIKANIA_CLIENT_EXPORT FontSdl {
private:
	using Handle = std::unique_ptr<TTF_Font, void (*)(TTF_Font*)>;

	Handle m_font;

public:
	FontSdl(std::string data, unsigned size);

	inline TTF_Font *font() noexcept
	{
		return m_font.get();
	}

	Size clip(const Font &self, const std::string &text) const;
};

/*
 * "Export" the backend.
 */
using BackendFont = FontSdl;

} // !malikania

#endif // !_MALIKANIA_FONT_SDL_H_

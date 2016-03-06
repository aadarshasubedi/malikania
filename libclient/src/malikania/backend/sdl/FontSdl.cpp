/*
 * FontSdl.cpp -- font object (SDL2 implementation)
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

#include <malikania/backend/sdl/FontSdl.h>

using namespace std::string_literals;

namespace malikania {

FontSdl::FontSdl(const std::string &data, unsigned size)
	: m_font(nullptr, nullptr)
	, m_size(size)
{
	/* Initialize the texture */
	auto rw = SDL_RWFromMem(const_cast<char *>(data.c_str()), data.length());

	if (rw == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	m_font = Handle(TTF_OpenFontRW(rw, true, m_size), TTF_CloseFont);

	if (m_font == NULL) {
		throw std::runtime_error(TTF_GetError());
	}
}

} // !malikania

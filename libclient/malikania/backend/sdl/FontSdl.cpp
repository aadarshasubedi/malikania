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

#include <malikania/Size.h>

#include <malikania/backend/sdl/CommonSdl.h>
#include <malikania/backend/sdl/FontSdl.h>

using namespace std::string_literals;

namespace malikania {

FontSdl::FontSdl(std::string data, unsigned size)
	: m_font(nullptr, nullptr)
{
	auto rw = sdl::RWFromBinary(std::move(data));

	if (rw == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	m_font = Handle(TTF_OpenFontRW(rw, true, size), TTF_CloseFont);

	if (m_font == NULL) {
		throw std::runtime_error(TTF_GetError());
	}
}

Size FontSdl::clip(const Font &, const std::string &text) const
{
	int width, height;

	if (TTF_SizeUTF8(m_font.get(), text.c_str(), &width, &height) != 0) {
		throw std::runtime_error(SDL_GetError());
	}

	return Size((unsigned)width, (unsigned)height);
}

} // !malikania

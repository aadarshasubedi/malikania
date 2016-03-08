/*
 * ImageSdl.cpp-- image object (SDL2 implementation)
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

#include <SDL_image.h>

#include <Window.h>

#include <malikania/backend/sdl/ImageSdl.h>

using namespace std::string_literals;

namespace malikania {

ImageSdl::ImageSdl(Image &, Window &window, const std::string &data)
	: m_texture(nullptr, nullptr)
{
	/* Initialize the texture */
	auto rw = SDL_RWFromMem(const_cast<char *>(data.c_str()), data.length());

	if (rw == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	m_texture = Handle(IMG_LoadTexture_RW(window.backend().renderer(), rw, true), SDL_DestroyTexture);

	if (m_texture == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	/* Store the size */
	int width, height;

	if (SDL_QueryTexture(m_texture.get(), nullptr, nullptr, &width, &height) < 0) {
		throw std::runtime_error(SDL_GetError());
	}

	m_size = Size((unsigned)width, (unsigned)height);
}

void ImageSdl::draw(Window &window, const Point &point)
{
	SDL_Rect target;

	target.x = (int)point.x();
	target.y = (int)point.y();
	target.w = (int)m_size.width();
	target.h = (int)m_size.height();

	if (SDL_RenderCopy(window.backend().renderer(), m_texture.get(), nullptr, &target) < 0) {
		throw std::runtime_error(SDL_GetError());
	}
}

void ImageSdl::draw(Window &window, const Rectangle &source, const Rectangle &target)
{
	SDL_Rect sr, st;

	sr.x = source.x();
	sr.y = source.y();
	sr.w = (int)source.width();
	sr.h = (int)source.height();

	st.x = target.x();
	st.y = target.y();
	st.w = (int)target.width();
	st.h = (int)target.height();

	/* Readjust .w, .h if null */
	if (source.isNull()) {
		sr.w = (int)m_size.width();
		sr.h = (int)m_size.height();
	}
	if (target.isNull()) {
		st.w = (int)m_size.width();
		st.h = (int)m_size.height();
	}

	if (SDL_RenderCopy(window.backend().renderer(), m_texture.get(), &sr, &st) < 0) {
		throw std::runtime_error(SDL_GetError());
	}
}

} // !malikania

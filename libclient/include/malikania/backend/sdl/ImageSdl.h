/*
 * ImageSdl.h -- image object (SDL2 implementation)
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

#ifndef _MALIKANIA_IMAGE_SDL_H_
#define _MALIKANIA_IMAGE_SDL_H_

#include <memory>

#include <SDL.h>

#include <malikania/Size.h>

namespace malikania {

class Image;
class Point;
class Window;

using TextureHandle = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>;

class ImageSdl {
private:
	TextureHandle m_texture;
	Size m_size;

public:
	ImageSdl(Image &image, Window &window, const std::string &data);

	inline SDL_Texture *texture() noexcept
	{
		return m_texture.get();
	}

	inline const Size &size() const noexcept
	{
		return m_size;
	}

	void draw(Window &window, const Point &position);
};

/*
 * "Export" the backend.
 */
using BackendImage = ImageSdl;

} // !malikania

#endif // !_MALIKANIA_IMAGE_SDL_H_

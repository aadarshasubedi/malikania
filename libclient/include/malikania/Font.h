/*
 * Font.h -- font object
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

#ifndef _MALIKANIA_FONT_H_
#define _MALIKANIA_FONT_H_

#include <string>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/FontSdl.h"
#endif

namespace malikania {

/**
 * @class Font
 * @brief Font object.
 */
class Font {
private:
	BackendFont m_backend;

public:
	/**
	 * Construct a font from binary data.
	 *
	 * @param data the raw data
	 * @param size the size
	 */
	inline Font(std::string path, unsigned size)
		: m_backend(path, size)
	{
	}

	/**
	 * Get the underlying backend.
	 *
	 * @return the backend
	 */
	inline BackendFont &backend() noexcept
	{
		return m_backend;
	}

	/**
	 * Overloaded function.
	 *
	 * @return the backend
	 */
	inline const BackendFont &backend() const noexcept
	{
		return m_backend;
	}
};

} // !malikania

#endif // _MALIKANIA_FONT_H_

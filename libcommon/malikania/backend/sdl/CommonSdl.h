/*
 * CommonSdl.h -- common SDL2 related code
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

#ifndef _COMMON_SDL_H_
#define _COMMON_SDL_H_

#include <SDL.h>

#include <string>

namespace malikania {

namespace sdl {

/**
 * Create a SDL_RWops that owns the binary data.
 *
 * This is a safe alternative to SDL_RWFromMem because it owns the memory pointed by data until it is closed. The
 * data is moved so there are no copies.
 *
 * The stream has read-only support and can not write.
 *
 * Seeking past-the-end or past-the-begin readjust the position to the end or begin respectively.
 *
 * @param data the data
 * @return the object or nullptr on errors
 */
SDL_RWops *RWFromBinary(std::string data) noexcept;

} // !sdl

} // !malikania

#endif // !_COMMON_SDL_H_
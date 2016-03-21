/*
 * CommonSdl.cpp -- common SDL2 related code
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

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <new>

#include <malikania/backend/sdl/CommonSdl.h>

namespace malikania {

namespace sdl {

namespace {

/*
 * RWFromBinary implementation
 * ------------------------------------------------------------------
 *
 * A little bit inspired by official SDL_RWFromMem implementation, largely modified to match our conventions and the
 * C++ code.
 */

class Buffer {
public:
	std::string m_data;
	std::uint64_t m_position;
	std::uint64_t m_length;

	inline Buffer(std::string data) noexcept
		: m_data(std::move(data))
		, m_position(0U)
		, m_length(m_data.length())
	{
	}
};

Sint64 size(SDL_RWops *ops)
{
	return reinterpret_cast<Buffer *>(ops->hidden.unknown.data1)->m_length;
}

Sint64 seek(SDL_RWops *ops, Sint64 offset, int whence)
{
	Buffer *data = reinterpret_cast<Buffer *>(ops->hidden.unknown.data1);
	Sint64 position;

	switch (whence) {
	case RW_SEEK_SET:
		position = offset;
		break;
	case RW_SEEK_CUR:
		position = data->m_position + offset;
		break;
	case RW_SEEK_END:
		position = data->m_length + offset;
		break;
	default:
		break;
	}

	if (position < 0)
		position = 0;
	else if ((std::uint64_t)position > data->m_length)
		position = data->m_length;

	return (data->m_position = position);
}

size_t read(SDL_RWops *ops, void *dst, size_t size, size_t number)
{
	Buffer *data = reinterpret_cast<Buffer *>(ops->hidden.unknown.data1);
	size_t total = number * size;
	size_t avail = data->m_length - data->m_position;

	if (number <= 0 || size <= 0 || ((total / number) != (size_t)size)) {
		return 0;
	}
	if (total > avail) {
		total = avail;
	}

	SDL_memcpy(dst, &data->m_data[data->m_position], total);

	data->m_position += total;

	return (total / size);
}

size_t write(SDL_RWops *, const void *, size_t, size_t)
{
	SDL_SetError("write not supported");

	return -1;
}

int close(SDL_RWops *ops)
{
	if (ops != nullptr) {
		delete reinterpret_cast<Buffer *>(ops->hidden.unknown.data1);
		SDL_FreeRW(ops);
	}

	return 0;
}

} // !namespace

SDL_RWops *RWFromBinary(std::string data) noexcept
{
	SDL_RWops *ops = SDL_AllocRW();

	if (ops == nullptr) {
		return nullptr;
	}

	ops->hidden.unknown.data1 = new (std::nothrow) Buffer(std::move(data));

	if (ops->hidden.unknown.data1 == nullptr) {
		SDL_SetError(std::strerror(errno));
		SDL_FreeRW(ops);
		return nullptr;
	}

	ops->type = SDL_RWOPS_UNKNOWN;
	ops->seek = seek;
	ops->size = size;
	ops->read = read;
	ops->write = write;
	ops->close = close;

	return ops;
}

} // !sdl

} // !malikania
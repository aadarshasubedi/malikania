/*
 * LoaderDirectory.cpp -- wrapper to load directories
 *
 * Copyright (c) 2013, 2014, 2015 Malikania Authors
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
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include "ResourcesLocator.h"

namespace malikania {

ResourcesLocatorDirectory::ResourcesLocatorDirectory(std::string path) noexcept
	: m_path(std::move(path))
{
}

std::string ResourcesLocatorDirectory::read(const std::string &id)
{
	std::ifstream in(m_path + "/" + id);

	if (!in)
		throw std::runtime_error(std::strerror(errno));

	return std::string(std::istreambuf_iterator<char>(in.rdbuf()), std::istreambuf_iterator<char>());
}

std::unique_ptr<std::istream> ResourcesLocatorDirectory::open(const std::string &id)
{
	auto ptr = std::make_unique<std::ifstream>(m_path + "/" + id);

	if (!(*ptr))
		throw std::runtime_error(std::strerror(errno));

	return ptr;
}

} // !malikania

/*
 * Game.cpp -- basic game class
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

#include <stdexcept>

#include "Game.h"
#include "Json.h"

namespace malikania {

namespace {

void check(const json::Object &object, const std::vector<std::string> &properties)
{
	for (const auto &prop : properties) {
		if (!object.contains(prop)) {
			throw std::invalid_argument{"missing `" + prop + "' property"};
		}
	}
}

} // !namespace

Game::Game(const json::Document &document)
{
	if (!document.isObject()) {
		throw std::invalid_argument{"invalid game metadata"};
	}

	json::Object object = document.toObject();

	/* Verify fields are present */
	check(object, {"name", "authors", "version", "requires"});

	/* Read fields */
	m_name = object["name"].toString();
	m_version = object["version"].toString();
	m_requires = object["requires"].toString();

	/* Authors is an array or a simple string */
	if (object["authors"].isArray()) {
		for (const auto &value : object["authors"].toArray()) {
			m_authors.push_back(value.toString());
		}
	} else {
		m_authors.push_back(object["authors"].toString());
	}
}

} // !malikania

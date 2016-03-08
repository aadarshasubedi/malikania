/*
 * ResourcesLoader.cpp -- load shared resources files
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

#include <cassert>

#include "Game.h"
#include "ResourcesLoader.h"
#include "ResourcesLocator.h"

namespace malikania {

void ResourcesLoader::requires(const std::string &id,
			       const json::Value &object,
			       const std::unordered_map<std::string, json::Type> props) const
{
	assert(object.isObject());

	for (const auto &pair : props) {
		auto it = object.find(pair.first);

		if (it == object.end() || it->typeOf() != pair.second) {
			std::string msg = id + ": missing '" + pair.first + "' property (";

			switch (pair.second) {
			case json::Type::Array:
				msg += "array";
				break;
			case json::Type::Boolean:
				msg += "boolean";
				break;
			case json::Type::Int:
				msg += "int";
				break;
			case json::Type::Object:
				msg += "object";
				break;
			case json::Type::Real:
				msg += "real";
				break;
			case json::Type::String:
				msg += "string";
				break;
			default:
				break;
			}

			msg += " expected)";

			throw std::runtime_error(std::move(msg));
		}
	}
}

std::string ResourcesLoader::requireString(const std::string &id,
					   const json::Value &object,
					   const std::string &property) const
{
	assert(object.isObject());

	auto it = object.find(property);

	if (it == object.end() || !it->isString()) {
		throw std::runtime_error(id + ": missing '" + property + "' property (string expected)");
	}

	return it->toString();
}

ResourcesLoader::ResourcesLoader(ResourcesLocator &locator)
	: m_locator(locator)
{
}

Game ResourcesLoader::loadGame() const
{
	json::Value value = json::fromString(m_locator.read("game.json"));

	if (!value.isObject())
		throw std::runtime_error("game.json: not a JSON object");

	requires("game.json", value, {
		{ "name", json::Type::String },
		{ "version", json::Type::String },
		{ "requires", json::Type::String }
	});

	return Game(value["name"].toString(),
		    value["version"].toString(),
		    value["requires"].toString(),
		    value.valueOr("license", json::Type::String, "").toString(),
		    value.valueOr("author", json::Type::String, "").toString());
}

} // !malikania

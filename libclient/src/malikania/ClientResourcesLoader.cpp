/*
 * ClientResourcesLoader.cpp -- load shared resources files for the client
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

#include <malikania/Animation.h>
#include <malikania/Size.h>
#include <malikania/Sprite.h>

#include "ClientResourcesLoader.h"

namespace malikania {

Image ClientResourcesLoader::loadImage(const std::string &id)
{
	return Image(m_window, locator().read(id));
}

Sprite ClientResourcesLoader::loadSprite(const std::string &id)
{
	json::Value value = json::fromString(locator().read(id));

	if (!value.isObject()) {
		throw std::runtime_error(id + ": not a JSON object");
	}

	return Sprite(
		loadImage(requireString(id, value, "image")),
		requireSize(id, value, "cell"),
		getSize(id, value, "size"),
		getSize(id, value, "space"),
		getSize(id, value, "margin")
	);
}

Animation ClientResourcesLoader::loadAnimation(const std::string &id)
{
	json::Value value = json::fromString(locator().read(id));

	if (!value.isObject()) {
		throw std::runtime_error(id + ": not a JSON object");
	}

	// TODO: return all resources as shared_ptr or make a cache for sprites.
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(loadSprite(requireString(id, value, "sprite")));

	/* Load all frames */
	json::Value property = value["frames"];

	if (!property.isArray()) {
		throw std::runtime_error(id + ": missing 'frames' property (array expected)");
	}

	std::vector<AnimationFrame> frames;

	for (auto it = property.begin(); it != property.end(); ++it) {
		if (!it->isObject()) {
			throw std::runtime_error(id + ": frame " + std::to_string(it.index()) + ": not a JSON object");
		}

		auto delay = it->find("delay");

		if (delay == it->end() || !delay->isInt()) {
			throw std::runtime_error(id + ": frame " + std::to_string(it.index()) +
			     ": missing 'delay' property (int expected)");
		}

		frames.emplace_back(delay->toInt());
	}

	return Animation(std::move(sprite), std::move(frames));
}

} // !malikania

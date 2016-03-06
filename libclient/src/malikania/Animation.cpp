/*
 * Animation.cpp -- animation description
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

#include "Animation.h"
#include "Window.h"

namespace malikania {

#if 0

void Animation::checkJSONFormat(const JsonObject& json)
{
	std::string prependErrorMessage = "Couldn't parse JSON Animation object: ";

	if (json.isObject()) {

		// Check required keys
		if (!json.contains("sprite")) {
			throw std::runtime_error(prependErrorMessage + "it doesn't contains the \"sprite\" required key");
		}
		if (!json.contains("alias")) {
			throw std::runtime_error(prependErrorMessage + "it doesn't contains \"alias\" required key");
		}
		if (!json.contains("frames")) {
			throw std::runtime_error(prependErrorMessage + "it doesn't contains \"frames\" required key");
		}

		// Check values types
		JsonValue image = json["sprite"];
		if (!image.isString()) {
			throw std::runtime_error(prependErrorMessage + "\"sprite\" must be a valid String");
		}
		JsonValue alias = json["alias"];
		if (!alias.isString()) {
			throw std::runtime_error(prependErrorMessage + "\"alias\" must be a valid String");
		}
		JsonValue frames = json["frames"];
		if (!frames.isArray()) {
			throw std::runtime_error(prependErrorMessage + "\"frames\" must be a valid Array");
		}
		for (JsonValue frameValue : frames.toArray()) {
			if (!frameValue.isObject()) {
				throw std::runtime_error(prependErrorMessage + "\"frames\" values have to be valid Object");
			}
			if (!frameValue.toObject().contains("delay")) {
				throw std::runtime_error(prependErrorMessage + "a \"frames\" value doesn't contains \"delay\" required key");
			}
			if (!frameValue.toObject()["delay"].isInteger()) {
				throw std::runtime_error(prependErrorMessage + "a \"frames\" value \"delay\" must be a valid Integer");
			}
			if (!frameValue.toObject().contains("cell")) {
				throw std::runtime_error(prependErrorMessage + "a \"frames\" value doesn't contains \"cell\" required key");
			}
			if (!frameValue.toObject()["cell"].isInteger()) {
				throw std::runtime_error(prependErrorMessage + "a \"frames\" value \"cell\" must be a valid Integer");
			}
		}
	} else {
		throw std::runtime_error(prependErrorMessage + "passed JSON is not an object");
	}
}

Animation::Animation(Sprite sprite, std::string alias, std::vector<std::map<std::string, int>> frames)
	: m_sprite(std::move(sprite))
	, m_name(std::move(alias))
	, m_frames(std::move(frames))
{
}

Animation Animation::fromJson(Window &window, const JsonObject &jsonAnimation)
{
	Animation::checkJSONFormat(jsonAnimation);

	// TODO
	//Sprite sprite = window.sprite(jsonAnimation["sprite"]);

	// FIXME Temporary, just for test
	Sprite sprite = Sprite::fromJson(window, JsonDocument(
		"{\"image\": \"resources/images/mokodemo.png\", \"alias\": \"testSprite\", \"cell\": [300, 300], \"size\": [1200, 900]}"
	 ).toObject());
	// !FIXME Temporary, just for test

	std::vector<std::map<std::string, int>> frames;
	for (JsonValue frame : jsonAnimation["frames"].toArray()) {
		std::map<std::string, int> frameMap;
		frameMap["delay"] = frame.toObject()["delay"].toInteger();
		frameMap["cell"] = frame.toObject()["cell"].toInteger();
		frames.push_back(frameMap);
	}

	return Animation(std::move(sprite), jsonAnimation["alias"].toString(), std::move(frames));
}

#endif

Animation::Animation(std::shared_ptr<Sprite> sprite, std::vector<AnimationFrame> frames) noexcept
	: m_sprite(std::move(sprite))
	, m_frames(std::move(frames))
{
	assert(m_sprite);
}

}// !malikania

#include "Sprite.h"
#include "Window.h"

namespace malikania {

#if 0

void Sprite::checkJSONFormat(const json::Value& json)
{
	std::string prependErrorMessage = "Couldn't parse JSON Sprite object: ";

	if (json.isObject()) {

		// Check required keys
		if (!json.contains("image")) {
			throw std::runtime_error(prependErrorMessage + "it doesn't contains the \"image\" required key");
		}
		if (!json.contains("alias")) {
			throw std::runtime_error(prependErrorMessage + "it doesn't contains \"alias\" required key");
		}
		if (!json.contains("cell")) {
			throw std::runtime_error(prependErrorMessage + "it doesn't contains \"cell\" required key");
		}

		// Check values types
		JsonValue image = json["image"];
		if (!image.isString()) {
			throw std::runtime_error(prependErrorMessage + "\"image\" must be a valid String");
		}
		JsonValue alias = json["alias"];
		if (!alias.isString()) {
			throw std::runtime_error(prependErrorMessage + "\"alias\" must be a valid String");
		}
		JsonValue cell = json["cell"];
		if (!cell.isArray()) {
			throw std::runtime_error(prependErrorMessage + "\"cell\" must be a valid Array");
		}
		if (cell.toArray().size() != 2) {
			throw std::runtime_error(prependErrorMessage + "\"cell\" must have 2 values");
		}
		for (JsonValue cellValue : cell.toArray()) {
			if (!cellValue.isInteger()) {
				throw std::runtime_error(prependErrorMessage + "\"cell\" values have to be valid Integer");
			}
		}

		// Check non required values only if they are actually in JSON object
		if (json.contains("size")) {
			JsonValue size = json["size"];
			if (!size.isArray()) {
				throw std::runtime_error(prependErrorMessage + "\"size\" must be a valid Array");
			}
			if (size.toArray().size() != 2) {
				throw std::runtime_error(prependErrorMessage + "\"size\" must have 2 values");
			}
			for (JsonValue sizeValue : size.toArray()) {
				if (!sizeValue.isInteger()) {
					throw std::runtime_error(prependErrorMessage + "\"size\" values have to be valid Integer");
				}
			}
		}
		if (json.contains("space")) {
			JsonValue space = json["space"];
			if (!space.isArray()) {
				throw std::runtime_error(prependErrorMessage + "\"space\" must be a valid Array");
			}
			if (space.toArray().size() != 2) {
				throw std::runtime_error(prependErrorMessage + "\"space\" must have 2 values");
			}
			for (JsonValue spaceValue : space.toArray()) {
				if (!spaceValue.isInteger()) {
					throw std::runtime_error(prependErrorMessage + "\"space\" values have to be valid Integer");
				}
			}
		}
		if (json.contains("margin")) {
			JsonValue margin = json["margin"];
			if (!margin.isArray()) {
				throw std::runtime_error(prependErrorMessage + "\"margin\" must be a valid Array");
			}
			if (margin.toArray().size() != 2) {
				throw std::runtime_error(prependErrorMessage + "\"margin\" must have 2 values");
			}
			for (JsonValue marginValue : margin.toArray()) {
				if (!marginValue.isInteger()) {
					throw std::runtime_error(prependErrorMessage + "\"margin\" values have to be valid Integer");
				}
			}
		}
	} else {
		throw std::runtime_error(prependErrorMessage + "passed JSON is not an object");
	}
}

#endif

Sprite::Sprite(Image image, std::string alias, Size cell, Size size, Size space, Size margin)
	: m_image(std::move(image))
	, m_name(std::move(alias))
	, m_cell(std::move(cell))
	, m_size(std::move(size))
	, m_space(std::move(space))
	, m_margin(std::move(margin))
{
}

#if 0

Sprite Sprite::fromJson(Window &window, const json::Value &jsonSprite)
{
	Sprite::checkJSONFormat(jsonSprite);
	Size cell({jsonSprite["cell"].toArray()[0].toInteger(), jsonSprite["cell"].toArray()[1].toInteger()});

	int width = 0;
	int height = 0;

	Image image(window, jsonSprite["image"].toString());

	if (jsonSprite.contains("size")) {
		width = jsonSprite["size"].toArray()[0].toInteger();
		height = jsonSprite["size"].toArray()[1].toInteger();
	} else {
		throw std::runtime_error("Error \"size\" of Sprite must be present, it's marked non required but for now Sprite need it");
	}
// TODO: find a workaround, only works with SDL now
#if 0
	else {
		SDL_QueryTexture(image.backend().texture(), nullptr, nullptr, &width, &height);
	}
#endif
	image.setSize(Size({width, height}));

	Size space;
	if (jsonSprite.contains("space")) {
		space.width = jsonSprite["space"].toArray()[0].toInteger();
		space.height = jsonSprite["space"].toArray()[1].toInteger();
	}

	Size margin;
	if (jsonSprite.contains("margin")) {
		margin.width = jsonSprite["margin"].toArray()[0].toInteger();
		margin.height = jsonSprite["margin"].toArray()[1].toInteger();
	}

	return Sprite(std::move(image), jsonSprite["alias"].toString(), std::move(cell)
			, Size({width, height}), std::move(space), std::move(margin));
}

#endif

void Sprite::draw(Window &window, int index, const Rectangle &rectangle)
{
	std::string prependErrorMessage = "Couldn't draw image from Sprite " + m_name + ": ";

	// Check if number of image by line is correct
	if ((m_size.width - m_margin.width) % (m_cell.width + m_space.width) != 0) {
		throw std::runtime_error(prependErrorMessage + "cell, margin and space don't fit the global sprite width correctly");
	}

	int numberOfImageByLine = (m_size.width - m_margin.width) / (m_cell.width + m_space.width);
	int verticalIndex = index / numberOfImageByLine;
	int horizontalIndex = index % numberOfImageByLine;

	// First, take margin width and height
	int leftPosition = m_margin.width;
	int topPosition = m_margin.height;

	// Second, take cell width and height
	leftPosition += (m_cell.height * horizontalIndex);
	topPosition += (m_cell.width * verticalIndex);

	// Third, take space (padding) between cells width and height
	leftPosition += (m_space.height * horizontalIndex);
	topPosition += (m_space.width * verticalIndex);

	m_backend.render(*this, window, leftPosition, topPosition, rectangle);
}

}// !malikania

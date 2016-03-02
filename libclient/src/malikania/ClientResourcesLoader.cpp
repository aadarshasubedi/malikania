#include <cassert>

#include <malikania/Json.h>
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

	if (!value.isObject())
		throw std::runtime_error(id + ": not a JSON object");

	return Sprite(
		loadImage(requireString(id, value, "image")),
		requireSize(id, value, "cell"),
		getSize(id, value, "size"),
		getSize(id, value, "space"),
		getSize(id, value, "margin")
	);
}

} // !malikania

#ifndef _MALIKANIA_DIRECTORY_LOADER_H_
#define _MALIKANIA_DIRECTORY_LOADER_H_

#include <stdexcept>

/**
 * @file DirectoryLoader.h
 * @brief Base class for directories loader
 */

#include "LoaderInterface.h"
#include "Json.h"

namespace malikania {

/**
 * @class DirectoryLoader
 * @brief Base class for directories loader
 *
 * This class provides a gameSettings() implementation.
 */
template <typename Interface>
class DirectoryLoader : public Interface {
protected:
	std::string m_path;

	JsonValue requires(const JsonObject &object, const std::string &name, JsonType type) const;

	JsonValue optional(const JsonObject &object, const std::string &name, JsonType type, JsonValue def = JsonValue()) const;

public:
	DirectoryLoader(std::string path);

	GameSettings gameSettings();
};

template <typename Interface>
JsonValue DirectoryLoader<Interface>::requires(const JsonObject &object, const std::string &name, JsonType type) const
{
	if (!object.contains(name))
		throw std::runtime_error("missing `" + name + "' field");

	JsonValue value = object[name];

	if (value.typeOf() != type)
		throw std::runtime_error("invalid type given");

	return value;
}

template <typename Interface>
JsonValue DirectoryLoader<Interface>::optional(const JsonObject &object, const std::string &name, JsonType type, JsonValue def) const
{
	JsonValue value = object[name];

	if (value.typeOf () != type)
		return def;

	return value;
}

template <typename Interface>
DirectoryLoader<Interface>::DirectoryLoader(std::string path)
	: m_path(std::move(path))
{
}

template <typename Interface>
GameSettings DirectoryLoader<Interface>::gameSettings()
{
	JsonDocument document = JsonDocument(std::ifstream(m_path + "/game.json"));

	if (!document.isObject())
		throw std::runtime_error("invalid game.json file");

	JsonObject object = document.toObject();
	GameSettings game;

	// Required fields
	game.name = requires(object, "name", JsonType::String).toString();
	game.version = requires(object, "version", JsonType::String).toString();
	game.requires = requires(object, "requires", JsonType::String).toString();

	JsonArray authors = requires(object, "authors", JsonType::Array).toArray();

	// TODO: change when iterators are in place
	if (authors.size() == 0) {
		throw std::runtime_error("field `authors' needs at least one element");
	}

	for (const JsonValue &v : authors) {
		if (!v.isString())
			throw std::runtime_error("author element is not a string");

		game.authors.push_back(v.toString());
	}

	// Optional fields
	game.license = optional(object, "license", JsonType::String).toString();

	return game;
}

} // !malikania

#endif // !_MALIKANIA_DIRECTORY_LOADER_H_


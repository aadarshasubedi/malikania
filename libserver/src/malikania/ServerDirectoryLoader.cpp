#include "ServerDirectoryLoader.h"

namespace malikania {

std::string jsonAsString(const JsonValue &value)
{
	std::string result;

	switch (value.typeOf()) {
	case JsonType::False:
		result = "false";
		break;
	case JsonType::True:
		result = "true";
		break;
	case JsonType::Integer:
		result = std::to_string(value.toInteger());
		break;
	case JsonType::Real:
		result = std::to_string(value.toReal());
		break;
	case JsonType::String:
		result = value.toString();
		break;
	default:
		break;
	}

	return result;
}

JsonValue jsonOr(const JsonObject &object, const std::string &key, JsonObject def)
{
	if (object.contains(key))
		return object[key];

	return def;
}

ServerSettingsNetwork readNetwork(const JsonObject &object)
{
	ServerSettingsNetwork network;

	network.port = jsonOr(object, "port", 9080).toInteger();
	network.portSecure = jsonOr(object, "port-secure", 9090).toInteger();

	return network;
}

ServerSettingsDatabase readDatabase(const JsonObject &object)
{
	ServerSettingsDatabase db;

	if (!object.contains("type"))
		throw std::runtime_error("missing `type'");

	for (const auto &v : object) {
		if (v.first == "type")
			continue;

		db.insert({v.first, jsonAsString(v.second)});
	}

	return db;
}

ServerSettings ServerDirectoryLoader::serverSettings()
{
	JsonDocument document(std::ifstream(m_path + "/server.json"));
	JsonObject object = document.toObject();
	ServerSettings settings;

	if (!object.isObject())
		throw std::runtime_error("invalid file");

	settings.database = readDatabase(object["database"].toObject());
	settings.network = readNetwork(object["network"].toObject());

	return settings;
}

} // !malikania

#ifndef _MALIKANIA_SERVER_SETTINGS_H_
#define _MALIKANIA_SERVER_SETTINGS_H_

#include <unordered_map>

namespace malikania {

class ServerSettingsNetwork {
public:
	std::string host{"*"};
	int port{0};
};

class ServerSettingsSsl {
public:
	int port{0};
	std::string privateKey;
	std::string certificate;
};

using ServerSettingsDatabase = std::unordered_map<std::string, std::string>;

class ServerSettings {
public:
	ServerSettingsNetwork network;
	ServerSettingsDatabase database;
	ServerSettingsSsl ssl;
};

} // !malikania

#endif // !_MALIKANIA_SERVER_SETTINGS_H_


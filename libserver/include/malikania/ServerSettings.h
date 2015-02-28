#ifndef _MALIKANIA_SERVER_SETTINGS_H_
#define _MALIKANIA_SERVER_SETTINGS_H_

#include <unordered_map>

namespace malikania {

class ServerSettingsNetwork {
public:
	int port{0};
	int portSecure{0};
};

using ServerSettingsDatabase = std::unordered_map<std::string, std::string>;

class ServerSettings {
public:
	ServerSettingsNetwork network;
	ServerSettingsDatabase database;
};

} // !malikania

#endif // !_MALIKANIA_SERVER_SETTINGS_H_


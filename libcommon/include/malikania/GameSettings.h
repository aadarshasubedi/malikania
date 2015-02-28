#ifndef _MALIKANIA_GAME_SETTINGS_H_
#define _MALIKANIA_GAME_SETTINGS_H_

/**
 * @file GameSettings.h
 * @brief Game settings definition
 */

#include <string>
#include <vector>

namespace malikania {

/**
 * @class GameSettings
 * @brief Game settings definition
 */
class GameSettings {
public:
	std::string name;
	std::vector<std::string> authors;
	std::string license;
	std::string version;
	std::string requires;
};

} // !malikania

#endif // !_MALIKANIA_GAME_SETTINGS_H_


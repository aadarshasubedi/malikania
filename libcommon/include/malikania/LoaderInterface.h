#ifndef _MALIKANIA_LOADER_INTERFACE_H_
#define _MALIKANIA_LOADER_INTERFACE_H_

/**
 * @file LoaderInterface.h
 * @brief Abstract interface class for loading game
 */

#include "GameSettings.h"

namespace malikania {

/**
 * @class LoaderInterface
 * @brief Abstract interface class for loading game
 */
class LoaderInterface {
public:
	/**
	 * Default constructor.
	 */
	LoaderInterface() = default;

	/**
	 * Default destructor.
	 */
	virtual ~LoaderInterface() = default;

	/**
	 * Get the game data definition.
	 *
	 * @return the game
	 */
	virtual GameSettings gameSettings() = 0;
};

} // !malikania

#endif // !_MALIKANIA_LOADER_INTERFACE_H_


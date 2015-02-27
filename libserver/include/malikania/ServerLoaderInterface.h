#ifndef _MALIKANIA_SERVER_LOADER_H_
#define _MALIKANIA_SERVER_LOADER_H_

#include <malikania/LoaderInterface.h>

#include "ServerSettings.h"

namespace malikania {

class ServerLoaderInterface : public LoaderInterface {
public:
	ServerLoaderInterface() = default;

	virtual ~ServerLoaderInterface() = default;

	virtual ServerSettings serverSettings() = 0;
};

} // !malikania

#endif // !_MALIKANIA_SERVER_LOADER_H_

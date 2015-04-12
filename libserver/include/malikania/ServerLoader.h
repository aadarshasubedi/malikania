#ifndef _MALIKANIA_SERVER_LOADER_H_
#define _MALIKANIA_SERVER_LOADER_H_

#include <malikania/Loader.h>

#include "ServerSettings.h"

namespace malikania {

class ServerLoader : public Loader {
public:
	ServerLoader() = default;

	virtual ~ServerLoader() = default;

	virtual ServerSettings serverSettings() = 0;
};

} // !malikania

#endif // !_MALIKANIA_SERVER_LOADER_H_

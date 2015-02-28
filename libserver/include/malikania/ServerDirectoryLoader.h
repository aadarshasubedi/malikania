#ifndef _SERVER_DIRECTORY_LOADER_H_
#define _SERVER_DIRECTORY_LOADER_H_

#include <malikania/DirectoryLoader.h>

#include "ServerLoaderInterface.h"
#include "ServerSettings.h"

namespace malikania {

class ServerDirectoryLoader : public DirectoryLoader<ServerLoaderInterface> {
public:
	using DirectoryLoader::DirectoryLoader;

	ServerSettings serverSettings() override;
};

} // !malikania

#endif // !_SERVER_DIRECTORY_LOADER_H_

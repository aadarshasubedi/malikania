#include "ServerDirectoryLoader.h"

namespace malikania {

ServerSettings ServerDirectoryLoader::serverSettings()
{
	JsonDocument document(std::ifstream(m_path + "/server,json"));

	if (!document.isObject())
		throw std::runtime_error("invalid file");


	return {};
}

} // !malikania

#include <iostream>

#include "ServerApplication.h"

namespace malikania {

void ServerApplication::run()
{
	GameSettings gs = m_loader->gameSettings();
	ServerSettings ss = m_loader->serverSettings();

	std::cout << "Game information:" << std::endl;
	std::cout << "  Name: " << gs.name << std::endl;
	std::cout << "  Version: " << gs.version << std::endl;

	std::cout << "Server settings:" << std::endl;
}

} // !malikania

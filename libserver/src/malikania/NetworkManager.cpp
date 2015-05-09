#include <malikania/SocketListener.h>

#include "NetworkManager.h"
#include "Id.h"
#include "ServerSettings.h"

namespace malikania {

void NetworkManager::processAccept(Socket &sc)
{
	try {
		if (sc == m_master) {

		} else {
			SocketSsl client = m_sslMaster.accept();
			unsigned id = Id::next();

			m_anonSsl.emplace(std::move(client), NetworkConnectionSsl(std::move(client), id));
		}
	} catch (const std::exception &ex) {
	}
}

void NetworkManager::processAnonymous(Socket &, int direction)
{
}

void NetworkManager::processClient(Socket &s, int direction)
{
}

bool NetworkManager::isMaster(const Socket &s) const noexcept
{
	return s == m_master || s == m_sslMaster;
}

bool NetworkManager::isAnonymous(const Socket &sc) const noexcept
{
	return m_anonTcp.count(sc) > 0 || m_anonSsl.count(sc) > 0;
}

void NetworkManager::run()
{
	SocketListener listener;

	while (m_running) {
		listener.clear();
		listener.set(m_master, SocketListener::Read);
		listener.set(m_sslMaster, SocketListener::Read);

		try {
			SocketStatus status = listener.select(250);

			if (isMaster(status.socket)) {
				processAccept(status.socket);
			} else if (isAnonymous(status.socket)) {
				processAnonymous(status.socket, status.direction);
			} else {
				processClient(status.socket, status.direction);
			}
		} catch (const SocketError &ex) {
			printf("error: %s\n", ex.what());
		}
	}
}

NetworkManager::NetworkManager(const ServerSettings &settings)
	: m_master(AF_INET, 0)
	, m_sslMaster(AF_INET, 0)
{
	//m_thread = std::thread(&NetworkManager::run, this);
}

} // !malikania

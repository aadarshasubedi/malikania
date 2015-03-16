#include <malikania/SocketListener.h>

#include "NetworkManager.h"
#include "ServerSettings.h"

namespace malikania {

void NetworkManager::accept(Socket &s)
{

}

void NetworkManager::process(Socket &s, int direction)
{

}

bool NetworkManager::isMaster(const Socket &s) const noexcept
{
	return s == m_master || s == m_sslMaster;
}

void NetworkManager::run()
{
	SocketListener listener;

	while (m_running) {
		listener.clear();
		listener.set(m_master, SocketListener::Read);
		listener.set(m_sslMaster, SocketListener::Read);

		try {
			SocketStatus st = listener.select();

			if (isMaster(status.socket)) {
				accept(status.socket);
			} else {
				process(status.socket, status.direction);
			}
		} catch (const SocketError &ex) {
		}
	}
}

NetworkManager::NetworkManager(const ServerSettings &settings)
	: m_master(AF_INET, 0)
	, m_sslMaster(AF_INET, 0)
{
	m_thread = std::thread(&NetworkManager::run, this);
}

} // !malikania

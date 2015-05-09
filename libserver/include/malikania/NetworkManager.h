#ifndef _MALIKANIA_NETWORK_MANAGER_H_
#define _MALIKANIA_NETWORK_MANAGER_H_

#include <atomic>
#include <map>
#include <memory>
#include <thread>
#include <vector>

#include <malikania/SocketSsl.h>
#include <malikania/SocketTcp.h>

#include "NetworkConnection.h"

namespace malikania {

class NetworkClient;
class ServerSettings;

class NetworkManager {
private:
	/* Master sockets */
	SocketTcp m_master;
	SocketSsl m_sslMaster;

	/* Thread */
	std::thread m_thread;
	std::atomic<bool> m_running{true};

	/* Identified clients */
	std::map<Socket, std::shared_ptr<NetworkClient>> m_clients;

	/* Not identified clients */
	std::map<Socket, NetworkConnectionTcp> m_anonTcp;
	std::map<Socket, NetworkConnectionSsl> m_anonSsl;

	void processAccept(Socket &);
	void processAnonymous(Socket &, int direction);
	void processClient(Socket &s, int direction);

	bool isMaster(const Socket &) const noexcept;
	bool isAnonymous(const Socket &) const noexcept;

	void run();

public:
	NetworkManager(const ServerSettings &);
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_MANAGER_H_

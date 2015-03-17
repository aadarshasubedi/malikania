#ifndef _MALIKANIA_NETWORK_MANAGER_H_
#define _MALIKANIA_NETWORK_MANAGER_H_

#include <atomic>
#include <map>
#include <memory>
#include <thread>
#include <vector>

#include <malikania/SocketSsl.h>
#include <malikania/SocketTcp.h>

namespace malikania {

class NetworkClient;
class ServerSettings;

class NetworkManager {
private:
	SocketTcp m_master;
	SocketSsl m_sslMaster;
	std::thread m_thread;
	std::map<Socket, std::shared_ptr<NetworkClient>> m_clients;
	std::atomic<bool> m_running{true};

	void accept(SocketTcp &s);
	void accept(SocketSsl &sc);
	void process(Socket &s, int direction);
	bool isMaster(const Socket &) const noexcept;
	void run();

public:
	NetworkManager(const ServerSettings &);
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_MANAGER_H_

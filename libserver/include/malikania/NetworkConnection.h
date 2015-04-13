#ifndef _MALIKANIA_NETWORK_CONNECTION_H_
#define _MALIKANIA_NETWORK_CONNECTION_H_

#include <mutex>
#include <string>

namespace malikania {

template <typename Sock>
class NetworkConnection {
private:
	Sock m_socket;
	std::mutex m_outputLock;
	std::string m_input;
	std::string m_output;

public:
	inline NetworkConnection(Sock socket) noexcept
		: m_socket(std::move(socket))
	{
	}

	inline void send(std::string output)
	{
		std::lock_guard<std::mutex> lock(m_outputLock);

		m_output += output;
	}
};

using NetworkConnectionTcp = NetworkConnection<SocketTcp>;
using NetworkConnectionSsl = NetworkConnection<SocketSsl>;

} // !malikania

#endif // !_MALIKANIA_NETWORK_CONNECTION_H_

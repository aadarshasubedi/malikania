#ifndef _MALIKANIA_NETWORK_CONNECTION_H_
#define _MALIKANIA_NETWORK_CONNECTION_H_

#include <malikania/ElapsedTimer.h>

#include <mutex>
#include <sstream>
#include <string>

namespace malikania {

template <typename Sock>
class NetworkConnection {
private:
	ElapsedTimer m_timer;
	Sock m_socket;
	std::string m_input;
	std::string m_output;

public:
	inline NetworkConnection(Sock socket) noexcept
		: m_socket(std::move(socket))
	{
	}

	inline void send(std::string output)
	{
		m_output += output;
	}
};

using NetworkConnectionTcp = NetworkConnection<SocketTcp>;

class NetworkConnectionSsl : public NetworkConnection<SocketSsl> {
private:
	unsigned m_id;

public:
	NetworkConnectionSsl(SocketSsl socket, unsigned id)
		: NetworkConnection<SocketSsl>(std::move(socket))
		, m_id(id)
	{
		// Send the identify request
		std::ostringstream oss;

		oss << "{"
		    <<   "\"command\"" << ":" << "\"identify-request\""
		    <<   "\"id\"" << ":" << m_id
		    << "}";

		send(oss.str());
	}
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_CONNECTION_H_

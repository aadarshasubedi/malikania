#ifndef _MALIKANIA_NETWORK_CONNECTION_H_
#define _MALIKANIA_NETWORK_CONNECTION_H_

#include <malikania/SocketSsl.h>
#include <malikania/SocketTcp.h>

namespace malikania {

template <typename Sock>
class NetworkConnectionBase {
private:
	Sock m_socket;
	std::string m_input;
	std::string m_output;

public:
	NetworkConnectionBase(Sock sc)
		: m_socket(std::move(sc))
	{
	}

	inline void append(std::string s)
	{
		m_output += std::move(s);
	}

	inline bool hasOutput() const noexcept
	{
		return !m_output.empty();
	}

	inline void read()
	{
		m_input += m_socket.recv(512);
	}

	inline void send()
	{
		m_output.erase(0, m_socket.send(m_output));
	}
};

using NetworkConnection = NetworkConnectionBase<SocketTcp>;
using NetworkConnectionSsl = NetworkConnectionBase<SocketSsl>;

} // !malikania

#endif // !_MALIKANIA_NETWORK_CONNECTION_H_

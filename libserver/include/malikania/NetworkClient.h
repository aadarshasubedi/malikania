#ifndef _MALIKANIA_NETWORK_CLIENT_H_
#define _MALIKANIA_NETWORK_CLIENT_H_

#include <malikania/SocketTcp.h>
#include <malikania/SocketSsl.h>

namespace malikania {

class NetworkClient {
private:
	SocketTcp m_socket;
	SocketSsl m_socketSsl;

public:
	NetworkClient(SocketTcp socket, SocketSsl ssl);
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_CLIENT_H_

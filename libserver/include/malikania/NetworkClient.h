#ifndef _MALIKANIA_NETWORK_CLIENT_H_
#define _MALIKANIA_NETWORK_CLIENT_H_

#include <malikania/SocketTcp.h>
#include <malikania/SocketSsl.h>

#include "NetworkConnection.h"

namespace malikania {

class NetworkClient {
private:
	NetworkConnectionTcp m_tcp;
	NetworkConnectionSsl m_ssl;

public:
	//NetworkClient(NetworkConnectionTcp nctcp, NetworkConnectionSsl ncssl);
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_CLIENT_H_


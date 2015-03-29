#include "NetworkClient.h"

namespace malikania {

NetworkClient::NetworkClient(NetworkConnectionTcp nctcp, NetworkConnectionSsl ncssl)
	: m_tcp(std::move(nctcp))
	, m_ssl(std::move(ncssl))
{
}

} // !malikania

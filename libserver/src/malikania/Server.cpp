#include "Server.h"

namespace malikania {

net::address::Ip Server::buildIp(const ServerSettings &st) const noexcept
{
	return net::address::Ip(st.address, st.port);
}

net::protocol::Tls Server::buildTls(const ServerSettings &st) const noexcept
{
	net::protocol::Tls tls;


	return tls;
}

Server::Server(const ServerSettings &st)
	: m_server(net::address::Ip)
{
}

} // !malikania

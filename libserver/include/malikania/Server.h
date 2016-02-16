#ifndef _MALIKANIA_SERVER_H_
#define _MALIKANIA_SERVER_H_

#include <cstdint>

#include <malikania/Sockets.h>

namespace malikania {

class ServerSettings {
public:
	std::string address{"*"};
	std::uint16_t port{3320};
};

class Server {
private:
	using StreamServer = net::StreamServer<net::address::Ip, net::protocol::Tls>;

	StreamServer m_server;

	net::address::Ip buildIp() const noexcept;
	net::protocol::Tls buildTls() const noexcept;

public:
	/**
	 * Construct a server.
	 *
	 * @param st the settings
	 */
	Server(const ServerSettings &st);

	/**
	 * Poll the next events.
	 *
	 * @param timeeout
	 */
	void poll(unsigned timeout = -1);
};

} // !malikania

#endif // !_MALIKANIA_SERVER_H_

#include <algorithm>
#include <random>

#if !defined(_WIN32)
#  include <netinet/tcp.h>
#endif

#include <malikania/Json.h>
#include <malikania/SocketAddress.h>
#include <malikania/SocketListener.h>

#include "NetworkClient.h"
#include "NetworkManager.h"
#include "Id.h"
#include "ServerSettings.h"

using namespace std::string_literals;

namespace malikania {

void NetworkManager::acceptStandard(SocketTcp &sc)
{
	assert(sc == m_master);

	SocketTcp client = sc.accept();
	client.setBlockMode(false);
	client.set(IPPROTO_TCP, TCP_NODELAY, 1);

	printf("network: <- unidentified client connected\n");

	m_listener.set(client, SocketListener::Read);
	m_anon.emplace(client, Anonymous(client));
}

void NetworkManager::acceptSsl(SocketSsl &sc)
{
	assert(sc == m_masterSsl);

	std::string rstring;
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<int> distribution('A', 'Z');

	rstring.resize(32);

	std::generate(rstring.begin(), rstring.end(), [&] () {
		return distribution(rng);
	});

	SocketSsl clientSsl = sc.accept();

	clientSsl.setBlockMode(false);
	clientSsl.set(IPPROTO_TCP, TCP_NODELAY, 1);

	printf("network: <- unidentified SSL client connected\n");

	AnonymousSsl anonymous(std::move(clientSsl), rstring, Id::next());
	anonymous.append(
		"{"
		"\"command\":\"identify-req\","
		"\"id\":" + std::to_string(anonymous.id()) + ","
		"\"hash\":\"" + rstring + "\""
		"}"
	);

	m_listener.set(anonymous.socket(), SocketListener::Write);
	m_anonSsl.emplace(anonymous.socket(), std::move(anonymous));
}

void NetworkManager::accept(Socket &sc)
{
	assert(isMaster(sc));

	try {
		if (sc == m_master) {
			acceptStandard(m_master);
		} else if (sc == m_masterSsl) {
			acceptSsl(m_masterSsl);
		}
	} catch (const std::exception &ex) {
		// TODO: logger
		printf("%s\n", ex.what());
	}
}

void NetworkManager::flushAnonymousStandard(Socket &sc, int flags)
{
	assert(isAnonymousStandard(sc));

	/* Nothing to write for standard anonymous socket */
	if (flags & SocketListener::Read) {
		m_anon.at(sc).read();
	}

	printf("network: <- unidentified message received\n");

	/* Check if user has identified */
	for (const std::string &msg : m_anon.at(sc).data()) {
		JsonObject object = JsonDocument(msg).toObject();

		/* Verify command */
		if (!object.contains("command")) {
			throw std::invalid_argument("missing `command' property");
		}
		if (object["command"].toString() != "identify-req") {
			throw std::invalid_argument("unsupported command: `"s + object["command"].toString() + "'"s);
		}

		/* Verify result */
		if (!object.contains("result")) {
			throw std::invalid_argument("missing `result` property");
		}

		std::string result = object["result"].toString();

		auto it = std::find_if(m_anonSsl.begin(), m_anonSsl.end(), [&] (const auto &pair) -> bool {
			return pair.second.result() == result;
		});

		if (it != m_anonSsl.end()) {
			printf("network: <- client successfully identified!\n");
			// TODO: logger debug, found client
			m_listener.remove(sc);
			m_listener.remove(it->second.socket());

			// TODO: store the socket into NetworkClient.
			m_lookup.emplace(sc, std::make_shared<NetworkClient>(it->second.id()));
			m_lookupSsl.emplace(it->second.socket(), std::make_shared<NetworkClient>(it->second.id()));

			m_anon.erase(sc);
			m_anonSsl.erase(it);
		}
	}
}

void NetworkManager::flushAnonymousSsl(Socket &sc, int flags)
{
	assert(isAnonymousSsl(sc));

	printf("network: -> unidentified SSL message about to be sent\n");

	/* Only write is needed for anonymous SSL */
	if (flags & SocketListener::Write) {
		if (m_anonSsl.at(sc).hasOutput()) {
			m_anonSsl.at(sc).send();
		}

		/* No more data to send? */
		if (!m_anonSsl.at(sc).hasOutput()) {
			m_listener.unset(sc, SocketListener::Write);
		}
	}
}

void NetworkManager::flushStandard(Socket &sc, int)
{
	assert(isStandard(sc));
}

void NetworkManager::flushSsl(Socket &sc, int)
{
	assert(isSsl(sc));
}

void NetworkManager::flush(Socket &sc, int flags)
{
	assert(!isMaster(sc));

	try {
		if (isAnonymousStandard(sc))
			flushAnonymousStandard(sc, flags);
		else if (isAnonymousSsl(sc))
			flushAnonymousSsl(sc, flags);
		else if (isStandard(sc))
			flushStandard(sc, flags);
		else if (isSsl(sc))
			flushSsl(sc, flags);
	} catch (const std::exception &ex) {
		// TODO: remove id if error with flushAnonymousSsl.

		sc.close();
		m_listener.remove(sc);

		// TODO: logger
		printf("error: %s\n", ex.what());
	}
}

bool NetworkManager::isAnonymousStandard(const Socket &sc) const noexcept
{
	return m_anon.count(sc) > 0;
}

bool NetworkManager::isAnonymousSsl(const Socket &sc) const noexcept
{
	return m_anonSsl.count(sc) > 0;
}

bool NetworkManager::isStandard(const Socket &sc) const noexcept
{
	return m_lookup.count(sc) > 0;
}

bool NetworkManager::isSsl(const Socket &sc) const noexcept
{
	return m_lookupSsl.count(sc) > 0;
}

bool NetworkManager::isMaster(const Socket &sc) const noexcept
{
	return sc == m_master || sc == m_masterSsl;
}

void NetworkManager::run()
{
	m_listener.clear();
	m_listener.set(m_master, SocketListener::Read);
	m_listener.set(m_masterSsl, SocketListener::Read);

	while (m_running) {
		try {
			SocketStatus status = m_listener.wait(250);

			if (isMaster(status.socket)) {
				accept(status.socket);
			} else {
				flush(status.socket, status.flags);
			}

			// TODO: kick, zombies
		} catch (const SocketError &ex) {
			// TODO: logger
			if (ex.code() != SocketError::Timeout)
				printf("error: %s\n", ex.what());
		}
	}
}

NetworkManager::NetworkManager(const ServerSettings &ss)
	: m_master(AF_INET, 0)
	, m_masterSsl(AF_INET, 0, SocketSslOptions(
		SocketSslOptions::TLSv1,
		ss.ssl.certificate,
		ss.ssl.privateKey
	))
{
	m_master.bind(address::Internet(ss.network.host, ss.network.port, AF_INET));
	m_master.listen(1024);
	m_masterSsl.bind(address::Internet(ss.network.host, ss.ssl.port, AF_INET));
	m_masterSsl.listen(1024);

	m_thread = std::thread([this] () {
		run();
	});
}

} // !malikania

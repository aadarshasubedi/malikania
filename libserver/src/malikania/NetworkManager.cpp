#include <algorithm>
#include <cassert>
#include <random>

#include <malikania/SocketListener.h>

#include "NetworkManager.h"
#include "Id.h"
#include "ServerSettings.h"

using namespace std::string_literals;

namespace malikania {

void NetworkManager::processAcceptStandard(SocketTcp &sc)
{
	SocketTcp client = sc.accept();

	m_anon.emplace(client, Anonymous(client));
}

void NetworkManager::processAcceptSsl(SocketSsl &sc)
{
	std::string rstring;
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<int> distribution('A', 'Z');

	rstring.resize(32);

	std::generate(rstring.begin(), rstring.end(), [&] () {
		return distribution(rng);
	});

	SocketSsl clientSsl = sc.accept();
	AnonymousSsl anon(std::move(clientSsl), rstring, Id::next());

	clientSsl.setBlockMode(false);
	anon.send(
		"{"
		"\"command\":\"identify-req\","
		"\"id\":" + std::to_string(anon.id()) + ","
		"\"hash\":\"" + rstring + "\""
		"}"
	);

	m_anonSsl.emplace(clientSsl, std::move(anon));
}

void NetworkManager::processAccept(Socket &sc)
{
	assert(isMaster(sc));

	try {
		if (sc == m_master) {
			processAcceptStandard(m_master);
		} else if (sc == m_masterSsl ){
			processAcceptSsl(m_masterSsl);
		}
	} catch (const std::exception &ex) {
		// TODO: logger
		printf("%s\n", ex.what());
	}
}

void NetworkManager::processAnonymous(Socket &sc, int flags)
{
	(void)sc;
	(void)flags;
}

void NetworkManager::processClient(Socket &sc, int flags)
{
	(void)sc;
	(void)flags;
}

void NetworkManager::process(Socket &sc, int flags)
{
	assert(!isMaster(sc));

	(void)flags;
}

bool NetworkManager::isMaster(const Socket &sc) const noexcept
{
	return sc == m_master || sc == m_masterSsl;
}

bool NetworkManager::isAnonymous(const Socket &sc) const noexcept
{
	return m_anon.count(sc) > 0 || m_anonSsl.count(sc) > 0;
}

void NetworkManager::run()
{
	SocketListener listener;

	while (m_running) {
		listener.clear();
		listener.set(m_master, SocketListener::Read);
		listener.set(m_masterSsl, SocketListener::Read);

		try {
			SocketStatus status = listener.select(250);

			if (isMaster(status.socket)) {
				processAccept(status.socket);
			} else {
				process(status.socket, status.direction);
			}
		} catch (const SocketError &ex) {
			printf("error: %s\n", ex.what());
		}
	}
}

NetworkManager::NetworkManager(const ServerSettings &settings)
	: m_master(AF_INET, 0)
	, m_masterSsl(AF_INET, 0)
{
	
}

} // !malikania

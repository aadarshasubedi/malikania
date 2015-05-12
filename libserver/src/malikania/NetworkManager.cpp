/*
 * NetworkManager.cpp -- processes clients
 *
 * Copyright (c) 2013, 2014, 2015 Malikania Authors
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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
	m_anon.emplace(client, UnidentifiedClient(client));
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

	SocketSsl clientSock = sc.accept();

	clientSock.setBlockMode(false);
	clientSock.set(IPPROTO_TCP, TCP_NODELAY, 1);

	printf("network: <- unidentified SSL client connected\n");

	UnidentifiedClientSsl client(std::move(clientSock), rstring, Id::next());
	client.append(
		"{"
		"\"command\":\"identify-req\","
		"\"id\":" + std::to_string(client.id()) + ","
		"\"hash\":\"" + rstring + "\""
		"}"
	);

	m_listener.set(client.socket(), SocketListener::Write);
	m_anonSsl.emplace(client.socket(), std::move(client));
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

void NetworkManager::flushUnidentifiedStandard(Socket &sc, int flags)
{
	assert(isUnidentifiedStandard(sc));

	/* Nothing to write for standard Unidentified socket */
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

			m_anon.erase(sc);
			m_anonSsl.erase(it);
		}
	}
}

void NetworkManager::flushUnidentifiedSsl(Socket &sc, int flags)
{
	assert(isUnidentifiedSsl(sc));

	printf("network: -> unidentified SSL message about to be sent\n");

	/* Only write is needed for Unidentified SSL */
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
		if (isUnidentifiedStandard(sc))
			flushUnidentifiedStandard(sc, flags);
		else if (isUnidentifiedSsl(sc))
			flushUnidentifiedSsl(sc, flags);
		else if (isStandard(sc))
			flushStandard(sc, flags);
		else if (isSsl(sc))
			flushSsl(sc, flags);
	} catch (const std::exception &ex) {
		// TODO: remove id if error with flushUnidentifiedSsl.

		sc.close();
		m_listener.remove(sc);

		// TODO: logger
		printf("error: %s\n", ex.what());
	}
}

bool NetworkManager::isUnidentifiedStandard(const Socket &sc) const noexcept
{
	return m_anon.count(sc) > 0;
}

bool NetworkManager::isUnidentifiedSsl(const Socket &sc) const noexcept
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
		/* Clean zombies */
		cleanUnidentified(m_anon);
		cleanUnidentified(m_anonSsl);

		try {
			SocketStatus status = m_listener.wait(250);

			if (isMaster(status.socket)) {
				accept(status.socket);
			} else {
				flush(status.socket, status.flags);
			}
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
	m_master.set(SOL_SOCKET, SO_REUSEADDR, 1);
	m_master.bind(address::Internet(ss.network.host, ss.network.port, AF_INET));
	m_master.listen(1024);
	m_masterSsl.set(SOL_SOCKET, SO_REUSEADDR, 1);
	m_masterSsl.bind(address::Internet(ss.network.host, ss.ssl.port, AF_INET));
	m_masterSsl.listen(1024);
}

NetworkManager::~NetworkManager()
{
	if (m_running) {
		throw std::runtime_error("thread still running");
	}

	/* Close all clients */
	cleanUnidentified(m_anon);
	cleanUnidentified(m_anonSsl);

	// TODO: same for authenticated
}

void NetworkManager::start()
{
	if (m_running) {
		throw std::runtime_error("thread already running");
	}

	m_running = true;
	m_thread = std::thread([this] () {
		run();
	});
}

void NetworkManager::stop()
{
	if (!m_running) {
		throw std::runtime_error("thread not running");
	}

	m_running = false;
	m_thread.join();
}

} // !malikania
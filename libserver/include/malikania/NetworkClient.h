/*
 * NetworkClient.h -- authenticated network client
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

#ifndef _MALIKANIA_NETWORK_CLIENT_H_
#define _MALIKANIA_NETWORK_CLIENT_H_

/**
 * @file NetworkClient.h
 * @brief Authenticated client
 */

#include <functional>
#include <mutex>
#include <string>

#include "NetworkConnection.h"

namespace malikania {

class NetworkClient {
private:
	unsigned m_id;
	NetworkConnection m_connection;
	NetworkConnectionSsl m_connectionSsl;

	std::function<void ()> m_onDisconnect;
	std::function<void (std::string)> m_onMessage;
	std::function<void (std::string)> m_onSslMessage;

public:
	inline NetworkClient(unsigned id, SocketTcp sock, SocketSsl sockSsl)
		: m_id(id)
		, m_connection(std::move(sock))
		, m_connectionSsl(std::move(sockSsl))
	{
	}

	inline void setOnDisconnect(std::function<void ()> func) noexcept
	{
		m_onDisconnect = std::move(func);
	}

	inline void setOnMessage(std::function<void (std::string)> func) noexcept
	{
		m_onMessage = std::move(func);
	}

	inline void setOnSslMessage(std::function<void (std::string)> func) noexcept
	{
		m_onSslMessage = std::move(func);
	}
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_CLIENT_H_


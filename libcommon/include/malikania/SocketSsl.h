/*
 * SocketSsl.h -- OpenSSL extension for sockets
 *
 * Copyright (c) 2013, David Demelier <markand@malikania.fr>
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

#ifndef _SOCKET_SSL_H_
#define _SOCKET_SSL_H_

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>

#include "Socket.h"

namespace malikania {

class SocketSslOptions {
public:
	enum {
		SSLv3	= (1 << 0),
		TLSv1	= (1 << 1),
		All	= (0xf)
	};

	unsigned short	method{All};
	std::string	certificate;
	std::string	privateKey;
	bool		verify{false};

	SocketSslOptions() = default;

	SocketSslOptions(unsigned short method, std::string certificate, std::string key, bool verify = false)
		: method(method)
		, certificate(std::move(certificate))
		, privateKey(std::move(key))
		, verify(verify)
	{
	}
};

class MALIKANIA_EXPORT SocketSslInterface : public SocketStandard {
private:
	using Ssl	= std::shared_ptr<SSL>;
	using SslContext = std::shared_ptr<SSL_CTX>;

	SslContext m_context;
	Ssl m_ssl;
	SocketSslOptions m_options;

public:
	SocketSslInterface(SSL_CTX *context, SSL *ssl, SocketSslOptions options = {});
	SocketSslInterface(SocketSslOptions options = {});
	void connect(Socket &s, const SocketAddress &address) override;
	void tryConnect(Socket &s, const SocketAddress &address, int timeout) override;
	Socket accept(Socket &s, SocketAddress &info) override;
	unsigned recv(Socket &s, void *data, unsigned len) override;
	unsigned recvfrom(Socket &s, void *data, unsigned len, SocketAddress &info) override;
	unsigned tryRecv(Socket &s, void *data, unsigned len, int timeout) override;
	unsigned tryRecvfrom(Socket &s, void *data, unsigned len, SocketAddress &info, int timeout) override;
	unsigned send(Socket &s, const void *data, unsigned len) override;
	unsigned sendto(Socket &s, const void *data, unsigned len, const SocketAddress &info) override;
	unsigned trySend(Socket &s, const void *data, unsigned len, int timeout) override;
	unsigned trySendto(Socket &s, const void *data, unsigned len, const SocketAddress &info, int timeout) override;
};

/**
 * @class SocketSsl
 * @brief SSL interface for sockets
 *
 * This class derives from Socket and provide SSL support through OpenSSL.
 *
 * It is perfectly safe to cast SocketSsl to Socket and vice-versa.
 */
class MALIKANIA_EXPORT SocketSsl : public Socket {
private:
	using Socket::Socket;

public:
	/**
	 * Initialize SSL library.
	 */
	static void init();

	/**
	 * Close SSL library.
	 */
	static void finish();

	/**
	 * Open a SSL socket with the specified family. Automatically
	 * use SOCK_STREAM as the type.
	 *
	 * @param family the family
	 * @param options the options
	 */
	SocketSsl(int family, SocketSslOptions options = {});
};

} // !malikania

#endif // !_SOCKET_SSL_H_

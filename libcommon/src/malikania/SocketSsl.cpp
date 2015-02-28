/*
 * SocketSsl.cpp -- OpenSSL extension for sockets
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

#include "SocketListener.h"
#include "SocketSsl.h"

namespace malikania {

using namespace direction;

namespace {

SSL_METHOD *method(int mflags)
{
	if (mflags & SocketSslOptions::All)
		return SSLv23_method();
	if (mflags & SocketSslOptions::SSLv3)
		return SSLv3_method();
	if (mflags & SocketSslOptions::TLSv1)
		return TLSv1_method();

	return SSLv23_method();
}

inline std::string sslError(int error)
{
	return ERR_reason_error_string(error);
}

} // !namespace

SocketSslInterface::SocketSslInterface(SSL_CTX *context, SSL *ssl, SocketSslOptions options)
	: SocketStandard()
	, m_context(context, SSL_CTX_free)
	, m_ssl(ssl, SSL_free)
	, m_options(std::move(options))
{
}

SocketSslInterface::SocketSslInterface(SocketSslOptions options)
	: SocketStandard()
	, m_options(std::move(options))
{
}

void SocketSslInterface::connect(Socket &s, const SocketAddress &address)
{
	SocketStandard::connect(s, address);

	// Context first
	auto context = SSL_CTX_new(method(m_options.method));

	m_context = SslContext(context, SSL_CTX_free);

	// SSL object then
	auto ssl = SSL_new(context);

	m_ssl = Ssl(ssl, SSL_free);

	SSL_set_fd(ssl, s.handle());

	auto ret = SSL_connect(ssl);

	if (ret <= 0) {
		auto error = SSL_get_error(ssl, ret);

		if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
			throw error::InProgress("connect", sslError(error), error, error);

		throw error::Error("accept", sslError(error), error);
	}
}

void SocketSslInterface::tryConnect(Socket &s, const SocketAddress &address, int timeout)
{
	try {
		// Initial try
		connect(s, address);
	} catch (const error::InProgress &ipe) {
		SocketListener listener{{s, ipe.direction()}};

		listener.select(timeout);

		// Second try
		connect(s, address);
	}
}

Socket SocketSslInterface::accept(Socket &s, SocketAddress &info)
{
	auto client = SocketStandard::accept(s, info);
	auto context = SSL_CTX_new(method(m_options.method));

	if (m_options.certificate.size() > 0)
		SSL_CTX_use_certificate_file(context, m_options.certificate.c_str(), SSL_FILETYPE_PEM);
	if (m_options.privateKey.size() > 0)
		SSL_CTX_use_PrivateKey_file(context, m_options.privateKey.c_str(), SSL_FILETYPE_PEM);
	if (m_options.verify && !SSL_CTX_check_private_key(context)) {
		client.close();
		throw error::Error("accept", "certificate failure", 0);
	}

	// SSL object
	auto ssl = SSL_new(context);

	SSL_set_fd(ssl, client.handle());

	auto ret = SSL_accept(ssl);

	if (ret <= 0) {
		auto error = SSL_get_error(ssl, ret);

		if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
			throw error::InProgress("accept", sslError(error), error, error);

		throw error::Error("accept", sslError(error), error);
	}

	return SocketSsl{client.handle(), std::make_shared<SocketSslInterface>(context, ssl)};
}

unsigned SocketSslInterface::recv(Socket &, void *data, unsigned len)
{
	auto nbread = SSL_read(m_ssl.get(), data, len);

	if (nbread <= 0) {
		auto error = SSL_get_error(m_ssl.get(), nbread);

		if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
			throw error::InProgress("accept", sslError(error), error, error);

		throw error::Error("recv", sslError(error), error);
	}

	return nbread;
}

unsigned SocketSslInterface::recvfrom(Socket &, void *, unsigned, SocketAddress &)
{
	throw error::Error("recvfrom", "SSL socket is not UDP compatible", 0);
}

unsigned SocketSslInterface::tryRecv(Socket &s, void *data, unsigned len, int timeout)
{
	SocketListener listener{{s, Read}};

	listener.select(timeout);

	return recv(s, data, len);
}

unsigned SocketSslInterface::tryRecvfrom(Socket &, void *, unsigned, SocketAddress &, int)
{
	throw error::Error("recvfrom", "SSL socket is not UDP compatible", 0);
}

unsigned SocketSslInterface::send(Socket &, const void *data, unsigned len)
{
	auto nbread = SSL_write(m_ssl.get(), data, len);

	if (nbread <= 0) {
		auto error = SSL_get_error(m_ssl.get(), nbread);

		if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
			throw error::InProgress("accept", sslError(error), error, error);

		throw error::Error("recv", sslError(error), error);
	}

	return nbread;
}

unsigned SocketSslInterface::sendto(Socket &, const void *, unsigned, const SocketAddress &)
{
	throw error::Error("sendto", "SSL socket is not UDP compatible", 0);
}

unsigned SocketSslInterface::trySend(Socket &s, const void *data, unsigned len, int timeout)
{
	SocketListener listener{{s, Write}};

	listener.select(timeout);

	return send(s, data, len);
}

unsigned SocketSslInterface::trySendto(Socket &, const void *, unsigned, const SocketAddress &, int)
{
	throw error::Error("sendto", "SSL socket is not UDP compatible", 0);
}

void SocketSsl::init()
{
	SSL_library_init();
	SSL_load_error_strings();
}

void SocketSsl::finish()
{
	ERR_free_strings();
}

SocketSsl::SocketSsl(int family, SocketSslOptions options)
	: Socket(family, SOCK_STREAM, 0)
{
	m_interface = std::make_shared<SocketSslInterface>(std::move(options));
}

} // !malikania

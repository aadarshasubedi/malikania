/*
 * Socket.cpp -- portable C++ socket wrappers
 *
 * Copyright (c) 2013, 2014 David Demelier <markand@malikania.fr>
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

#include <cstring>

#include "Socket.h"
#include "SocketAddress.h"
#include "SocketListener.h"

namespace malikania {

using namespace direction;

/* --------------------------------------------------------
 * Socket exceptions
 * -------------------------------------------------------- */

namespace error {

/* --------------------------------------------------------
 * Error
 * -------------------------------------------------------- */

Error::Error(std::string function, std::string error, int code)
	: m_function(std::move(function))
	, m_error(std::move(error))
	, m_code(std::move(code))
{
	m_shortcut = m_function + ": " + m_error;
}

const std::string &Error::function() const noexcept
{
	return m_function;
}

const std::string &Error::error() const noexcept
{
	return m_error;
}

int Error::code() const noexcept
{
	return m_code;
}

const char *Error::what() const noexcept
{
	return m_shortcut.c_str();
}

/* --------------------------------------------------------
 * InProgress
 * -------------------------------------------------------- */

InProgress::InProgress(std::string func, std::string reason, int code, int direction)
	: Error(std::move(func), std::move(reason), code)
	, m_direction(direction)
{
}

int InProgress::direction() const noexcept
{
	return m_direction;
}

/* --------------------------------------------------------
 * WouldBlock
 * -------------------------------------------------------- */

WouldBlock::WouldBlock(std::string func, std::string reason, int code, int direction)
	: Error(std::move(func), std::move(reason), code)
	, m_direction(direction)
{
}

int WouldBlock::direction() const noexcept
{
	return m_direction;
}

/* --------------------------------------------------------
 * Timeout
 * -------------------------------------------------------- */

Timeout::Timeout(std::string func)
	: m_error(func + ": Timeout exception")
{
}

const char *Timeout::what() const noexcept
{
	return m_error.c_str();
}

} // !error

/* --------------------------------------------------------
 * Socket implementation
 * -------------------------------------------------------- */

void Socket::init()
{
#if defined(_WIN32)
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

void Socket::finish()
{
#if defined(_WIN32)
	WSACleanup();
#endif
}

/* --------------------------------------------------------
 * System dependent code
 * -------------------------------------------------------- */

#if defined(_WIN32)

std::string Socket::syserror(int errn)
{
	LPSTR str = nullptr;
	std::string errmsg = "Unknown error";

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errn,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&str, 0, NULL);


	if (str) {
		errmsg = std::string(str);
		LocalFree(str);
	}

	return errmsg;
}

#else

#include <cerrno>

std::string Socket::syserror(int errn)
{
	return strerror(errn);
}

#endif

std::string Socket::syserror()
{
#if defined(_WIN32)
	return syserror(WSAGetLastError());
#else
	return syserror(errno);
#endif
}

/* --------------------------------------------------------
 * SocketStandard clear implementation
 * -------------------------------------------------------- */

void SocketStandard::bind(Socket &s, const SocketAddress &addr)
{
	auto &sa = addr.address();
	auto addrlen = addr.length();

	if (::bind(s.handle(), (sockaddr *)&sa, addrlen) == SOCKET_ERROR)
		throw error::Error("bind", Socket::syserror(), errno);
}

void SocketStandard::connect(Socket &s, const SocketAddress &addr)
{
	if (m_connected)
		return;

	auto &sa = addr.address();
	auto addrlen = addr.length();

	if (::connect(s.handle(), (sockaddr *)&sa, addrlen) == SOCKET_ERROR) {
		/*
		 * Determine if the error comes from a non-blocking connect that cannot be
		 * accomplished yet.
		 */
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			throw error::InProgress("connect", Socket::syserror(WSAEWOULDBLOCK), WSAEWOULDBLOCK, Write);

		throw error::Error("connect", Socket::syserror(WSAEWOULDBLOCK), WSAGetLastError());
#else
		if (errno == EINPROGRESS)
			throw error::InProgress("connect", Socket::syserror(EINPROGRESS), EINPROGRESS, Write);

		throw error::Error("connect", Socket::syserror(), errno);
#endif
	}

	m_connected = true;
}

void SocketStandard::tryConnect(Socket &s, const SocketAddress &address, int timeout)
{
	if (m_connected)
		return;

	// Initial try
	try {
		connect(s, address);
	} catch (const error::InProgress &) {
		SocketListener listener{{s, Write}};

		listener.select(timeout);

		// Socket is writable? Check if there is an error
		auto error = s.get<int>(SOL_SOCKET, SO_ERROR);

		if (error)
			throw error::Error("connect", Socket::syserror(error), error);
	}

	m_connected = true;
}

Socket SocketStandard::accept(Socket &s, SocketAddress &info)
{
	Socket::Handle handle;

	// Store the information
	sockaddr_storage address;
	socklen_t addrlen;

	addrlen = sizeof (sockaddr_storage);
	handle = ::accept(s.handle(), (sockaddr *)&address, &addrlen);

	if (handle == INVALID_SOCKET) {
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			throw error::WouldBlock("accept", Socket::syserror(WSAEWOULDBLOCK), WSAEWOULDBLOCK, Read);

		throw error::Error("accept", Socket::syserror(), WSAGetLastError());
#else
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			throw error::WouldBlock("accept", Socket::syserror(EWOULDBLOCK), EWOULDBLOCK, Read);

		throw error::Error("accept", Socket::syserror(), errno);
#endif
	}

	// Usually accept works only with SOCK_STREAM
	info = SocketAddress(address, addrlen);

	return Socket(handle, std::make_shared<SocketStandard>());
}

Socket SocketStandard::tryAccept(Socket &s, SocketAddress &info, int timeout)
{
	SocketListener listener{{s, Read}};

	listener.select(timeout);

	return accept(s, info);
}

void SocketStandard::listen(Socket &s, int max)
{
	if (::listen(s.handle(), max) == SOCKET_ERROR)
		throw error::Error("listen", Socket::syserror(), errno);
}

unsigned SocketStandard::recv(Socket &s, void *data, unsigned dataLen)
{
	int nbread;

	nbread = ::recv(s.handle(), (Socket::Arg)data, dataLen, 0);
	if (nbread == SOCKET_ERROR) {
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			throw error::WouldBlock("recv", Socket::syserror(), WSAEWOULDBLOCK, Read);

		throw error::Error("recv", Socket::syserror(), WSAGetLastError());
#else
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			throw error::WouldBlock("recv", Socket::syserror(), errno, Read);

		throw error::Error("recv", Socket::syserror(), errno);
#endif
	}

	return (unsigned)nbread;
}

unsigned SocketStandard::tryRecv(Socket &s, void *data, unsigned len, int timeout)
{
	SocketListener listener{{s, Read}};

	listener.select(timeout);

	return recv(s, data, len);
}

unsigned SocketStandard::send(Socket &s, const void *data, unsigned dataLen)
{
	int nbsent;

	nbsent = ::send(s.handle(), (Socket::ConstArg)data, dataLen, 0);
	if (nbsent == SOCKET_ERROR) {
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			throw error::WouldBlock("send", Socket::syserror(), WSAEWOULDBLOCK, Write);

		throw error::Error("send", Socket::syserror(), WSAGetLastError());
#else
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			throw error::WouldBlock("send", Socket::syserror(), errno, Write);

		throw error::Error("send", Socket::syserror(), errno);
#endif
	}

	return (unsigned)nbsent;
}

unsigned SocketStandard::trySend(Socket &s, const void *data, unsigned len, int timeout)
{
	SocketListener listener{{s, Write}};

	listener.select(timeout);

	return send(s, data, len);
}

unsigned SocketStandard::recvfrom(Socket &s, void *data, unsigned dataLen, SocketAddress &info)
{
	int nbread;

	// Store information
	sockaddr_storage address;
	socklen_t addrlen;

	addrlen = sizeof (struct sockaddr_storage);
	nbread = ::recvfrom(s.handle(), (Socket::Arg)data, dataLen, 0, (sockaddr *)&address, &addrlen);

	info = SocketAddress(address, addrlen);

	if (nbread == SOCKET_ERROR) {
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			throw error::WouldBlock("recvfrom", Socket::syserror(), WSAEWOULDBLOCK, Read);

		throw error::Error("recvfrom", Socket::syserror(), WSAGetLastError());
#else
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			throw error::WouldBlock("recvfrom", Socket::syserror(), errno, Read);

		throw error::Error("recvfrom", Socket::syserror(), errno);
#endif
	}

	return (unsigned)nbread;
}

unsigned SocketStandard::tryRecvfrom(Socket &s, void *data, unsigned len, SocketAddress &info, int timeout)
{
	SocketListener listener{{s, Read}};

	listener.select(timeout);

	return recvfrom(s, data, len, info);
}

unsigned SocketStandard::sendto(Socket &s, const void *data, unsigned dataLen, const SocketAddress &info)
{
	int nbsent;

	nbsent = ::sendto(s.handle(), (Socket::ConstArg)data, dataLen, 0, (const sockaddr *)&info.address(), info.length());
	if (nbsent == SOCKET_ERROR) {
#if defined(_WIN32)
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			throw error::WouldBlock("sendto", Socket::syserror(), WSAEWOULDBLOCK, Write);

		throw error::Error("sendto", Socket::syserror(), errno);
#else
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			throw error::WouldBlock("sendto", Socket::syserror(), errno, Write);

		throw error::Error("sendto", Socket::syserror(), errno);
#endif
	}

	return (unsigned)nbsent;
}

unsigned SocketStandard::trySendto(Socket &s, const void *data, unsigned len, const SocketAddress &info, int timeout)
{
	SocketListener listener{{s, Write}};

	listener.select(timeout);

	return sendto(s, data, len, info);
}

void SocketStandard::close(Socket &s)
{
	(void)closesocket(s.handle());

	m_connected = false;
}

/* --------------------------------------------------------
 * Socket code
 * -------------------------------------------------------- */

Socket::Socket()
	: m_interface(std::make_shared<SocketStandard>())
{
}

Socket::Socket(int domain, int type, int protocol)
	: Socket()
{
	m_handle = socket(domain, type, protocol);

	if (m_handle == INVALID_SOCKET)
		throw error::Error("socket", syserror(), errno);
}

Socket::Socket(Handle handle, std::shared_ptr<SocketInterface> iface)
	: m_interface(std::move(iface))
	, m_handle(handle)
{
}

Socket::Handle Socket::handle() const
{
	return m_handle;
}

void Socket::blockMode(bool block)
{
#if defined(O_NONBLOCK) && !defined(_WIN32)
	int flags;

	if ((flags = fcntl(m_handle, F_GETFL, 0)) == -1)
		flags = 0;

	if (block)
		flags &= ~(O_NONBLOCK);
	else
		flags |= O_NONBLOCK;

	if (fcntl(m_handle, F_SETFL, flags) == -1)
		throw error::Error("blockMode", Socket::syserror(), errno);
#else
	unsigned long flags = (block) ? 0 : 1;

	if (ioctlsocket(m_handle, FIONBIO, &flags) == SOCKET_ERROR)
		throw error::Error("blockMode", Socket::syserror(), WSAGetLastError());
#endif
}

bool operator==(const Socket &s1, const Socket &s2)
{
	return s1.handle() == s2.handle();
}

bool operator<(const Socket &s1, const Socket &s2)
{
	return s1.handle() < s2.handle();
}

/*
 *
 */

Socket Socket::tryAccept(int timeout)
{
	SocketAddress dummy;

	return tryAccept(dummy, timeout);
}

Socket Socket::accept()
{
	SocketAddress dummy;

	return accept(dummy);
}

unsigned Socket::recvfrom(void *data, unsigned dataLen)
{
	SocketAddress dummy;

	return m_interface->recvfrom(*this, data, dataLen, dummy);
}

std::string Socket::recvfrom(unsigned count)
{
	SocketAddress dummy;

	return recvfrom(count, dummy);
}

} // !malikania

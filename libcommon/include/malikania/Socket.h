/*
 * Socket.h -- portable C++ socket wrappers
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

#ifndef _SOCKET_NG_H_
#define _SOCKET_NG_H_

#include <cstring>
#include <exception>
#include <memory>
#include <string>

#if defined(_WIN32)
#  include <WinSock2.h>
#  include <WS2tcpip.h>
#else
#  include <cerrno>

#  include <sys/ioctl.h>
#  include <sys/socket.h>
#  include <sys/types.h>

#  include <arpa/inet.h>

#  include <netinet/in.h>

#  include <fcntl.h>
#  include <netdb.h>
#  include <unistd.h>

#  define ioctlsocket(s, p, a)	::ioctl(s, p, a)
#  define closesocket(s)	::close(s)

#  define gai_strerrorA		gai_strerror

#  define INVALID_SOCKET	-1
#  define SOCKET_ERROR		-1
#endif

#include "Config.h"

namespace malikania {

class Socket;
class SocketAddress;

/**
 * Namespace for listener flags.
 */
namespace direction {

constexpr const int Read	= (1 << 0);	//!< Wants to read
constexpr const int Write	= (1 << 1);	//!< Wants to write

} // !direction

/**
 * Various errors.
 */
namespace error {

/**
 * Base error class, contains
 */
class MALIKANIA_EXPORT Error : public std::exception {
private:
	std::string m_function;
	std::string m_error;
	int m_code;
	std::string m_shortcut;

public:
	/**
	 * Construct a full error.
	 *
	 * @param function which function
	 * @param error the error
	 * @param code the native code
	 */
	Error(std::string function, std::string error, int code);

	/**
	 * Get the function which thrown an exception.
	 *
	 * @return the function name
	 */
	const std::string &function() const noexcept;

	/**
	 * Get the error string.
	 *
	 * @return the error
	 */
	const std::string &error() const noexcept;

	/**
	 * Get the native code. Use with care because it varies from the system,
	 * the type of socket and such.
	 *
	 * @return the code
	 */
	int code() const noexcept;

	/**
	 * Get a brief error message
	 *
	 * @return a message
	 */
	const char *what() const noexcept override;
};

/**
 * @class InProgress
 * @brief Operation cannot be accomplished now
 *
 * Usually thrown in a non-blocking connect call.
 */
class MALIKANIA_EXPORT InProgress final : public Error {
private:
	int m_direction;

public:
	/**
	 * Operation is in progress, contains the direction needed to complete
	 * the operation.
	 *
	 * The direction may be different from the requested operation because
	 * some sockets (e.g SSL) requires reading even when writing!
	 *
	 * @param func the function
	 * @param reason the reason
	 * @param code the native code
	 * @param direction the direction
	 */
	InProgress(std::string func, std::string reason, int code, int direction);

	/**
	 * Get the required direction for listening operation requires.
	 *
	 * @return the direction required
	 */
	int direction() const noexcept;
};

/**
 * @class WouldBlock
 * @brief The operation would block
 *
 * Usually thrown in a non-blocking connect send or receive.
 */
class MALIKANIA_EXPORT WouldBlock final : public Error {
private:
	int m_direction;

public:
	/**
	 * Operation would block, contains the direction needed to complete the
	 * operation.
	 *
	 * The direction may be different from the requested operation because
	 * some sockets (e.g SSL) requires reading even when writing!
	 *
	 * @param func the function
	 * @param reason the reason
	 * @param code the native code
	 * @param direction the direction
	 */
	WouldBlock(std::string func, std::string reason, int code, int direction);

	/**
	 * Get the required direction for listening operation requires.
	 *
	 * @return the direction required
	 */
	int direction() const noexcept;
};

/**
 * @class Timeout
 * @brief Describe a timeout expiration
 *
 * Usually thrown on timeout in SocketListener::select.
 */
class MALIKANIA_EXPORT Timeout final : public std::exception {
private:
	std::string m_error;

public:
	/**
	 * Timeout exception.
	 *
	 * @param func the function name
	 */
	Timeout(std::string func);

	/**
	 * The error message.
	 */
	const char *what() const noexcept override;
};

} // !error

/**
 * @class SocketInterface
 * @brief Interface to implement
 *
 * This class implements the socket functions.
 */
class MALIKANIA_EXPORT SocketInterface {
public:
	/**
	 * Default destructor.
	 */
	virtual ~SocketInterface() = default;

	/**
	 * Bind the socket.
	 *
	 * @param s the socket
	 * @param address the address
	 * @throw error::Failure on error
	 */
	virtual void bind(Socket &s, const SocketAddress &address) = 0;

	/**
	 * Close the socket.
	 *
	 * @param s the socket
	 */
	virtual void close(Socket &s) = 0;

	/**
	 * Try to connect to the specific address
	 *
	 * @param s the socket
	 * @param address the address
	 * @throw error::Failure on error
	 * @throw error::InProgress if the socket is marked non-blocking and connection cannot be established yet
	 */
	virtual void connect(Socket &s, const SocketAddress &address) = 0;

	/**
	 * Try to connect without blocking.
	 *
	 * @param s the socket
	 * @param address the address
	 * @param timeout the timeout in milliseconds
	 * @warning This function won't block only if the socket is marked non blocking
	 */
	virtual void tryConnect(Socket &s, const SocketAddress &address, int timeout) = 0;

	/**
	 * Accept a client.
	 *
	 * @param s the socket
	 * @param info the client information
	 * @return a client ready to use
	 * @throw error::Failure on error
	 */
	virtual Socket accept(Socket &s, SocketAddress &info) = 0;

	/**
	 * Try to accept a client with a timeout.
	 *
	 * @param s the socket
	 * @param info the client information
	 * @param timeout the timeout in milliseconds
	 */
	virtual Socket tryAccept(Socket &s, SocketAddress &info, int timeout) = 0;

	/**
	 * Listen to a specific number of pending connections.
	 *
	 * @param s the socket
	 * @param max the max number of clients
	 * @throw error::Failure on error
	 */
	virtual void listen(Socket &s, int max) = 0;

	/**
	 * Receive some data.
	 *
	 * @param s the socket
	 * @param data the destination pointer
	 * @param len max length to receive
	 * @return the number of bytes received
	 * @throw error::Failure on error
	 * @throw error::WouldBlock if the socket is marked non-blocking and the operation would block
	 */
	virtual unsigned recv(Socket &s, void *data, unsigned len) = 0;

	/**
	 * Try to receive data without blocking.
	 *
	 * @param s the socket
	 * @param data the destination pointer
	 * @param len max length to receive
	 * @param timeout the timeout in milliseconds
	 * @return the number of bytes received
	 * @throw error::Failure on error
	 */
	virtual unsigned tryRecv(Socket &s, void *data, unsigned len, int timeout) = 0;

	/**
	 * Receive from a connection-less socket.
	 *
	 * @param s the socket
	 * @param data the destination pointer
	 * @param dataLen max length to receive
	 * @param info the client information
	 * @return the number of bytes received
	 * @throw error::Failure on error
	 * @throw error::WouldBlock if the socket is marked non-blocking and the operation would block
	 */
	virtual unsigned recvfrom(Socket &s, void *data, unsigned len, SocketAddress &info) = 0;

	/**
	 * Try to receive data from a connection-less socket without blocking.
	 *
	 * @param s the socket
	 * @param data the data
	 * @param len max length to receive
	 * @param info the client information
	 * @param timeout the optional timeout in milliseconds
	 * @return the number of bytes received
	 * @throw error::Failure on error
	 */
	virtual unsigned tryRecvfrom(Socket &s, void *data, unsigned len, SocketAddress &info, int timeout) = 0;

	/**
	 * Send some data.
	 *
	 * @param s the socket
	 * @param data the data to send
	 * @param dataLen the data length
	 * @return the number of bytes sent
	 * @throw error::Failure on error
	 * @throw error::WouldBlock if the socket is marked non-blocking and the operation would block
	 */
	virtual unsigned send(Socket &s, const void *data, unsigned len) = 0;

	/**
	 * Try to send some data without blocking.
	 *
	 * @param s the socket
	 * @param data the data to send
	 * @param dataLen the data length
	 * @param timeout the optional timeout in milliseconds
	 * @return the number of bytes sent
	 * @throw error::Failure on error
	 * @throw error::WouldBlock if the socket is marked non-blocking and the operation would block
	 * @warning This function won't block only if the socket is marked non blocking
	 */
	virtual unsigned trySend(Socket &s, const void *data, unsigned len, int timeout) = 0;

	/**
	 * Send some data to a connection-less socket.
	 *
	 * @param s the socket
	 * @param data the data to send
	 * @param dataLen the data length
	 * @param address the address
	 * @return the number of bytes sent
	 * @throw error::Failure on error
	 * @throw error::WouldBlock if the socket is marked non-blocking and the operation would block
	 */
	virtual unsigned sendto(Socket &s, const void *data, unsigned len, const SocketAddress &info) = 0;

	/**
	 * Try to send some data to a connection-less without blocking.
	 *
	 * @param s the socket
	 * @param data the data to send
	 * @param dataLen the data length
	 * @param address the address
	 * @return the number of bytes sent
	 * @throw error::Failure on error
	 * @throw error::WouldBlock if the socket is marked non-blocking and the operation would block
	 * @warning This function won't block only if the socket is marked non blocking
	 */
	virtual unsigned trySendto(Socket &s, const void *data, unsigned len, const SocketAddress &info, int timeout) = 0;
};

/**
 * Standard interface for sockets.
 *
 * This will use standard clear functions:
 *
 *	bind(2)
 *	connect(2)
 *	close(2), closesocket on Windows,
 *	accept(2),
 *	listen(2),
 *	recv(2),
 *	recvfrom(2),
 *	send(2),
 *	sendto(2)
 */
class MALIKANIA_EXPORT SocketStandard : public SocketInterface {
protected:
	bool m_connected{false};

public:
	void bind(Socket &s, const SocketAddress &address) override;
	void close(Socket &s) override;
	void connect(Socket &s, const SocketAddress &address) override;
	void tryConnect(Socket &s, const SocketAddress &address, int timeout) override;
	Socket accept(Socket &s, SocketAddress &info) override;
	Socket tryAccept(Socket &s, SocketAddress &info, int timeout) override;
	void listen(Socket &s, int max) override;
	unsigned recv(Socket &s, void *data, unsigned len) override;
	unsigned tryRecv(Socket &s, void *data, unsigned len, int timeout) override;
	unsigned recvfrom(Socket &s, void *data, unsigned len, SocketAddress &info) override;
	unsigned tryRecvfrom(Socket &s, void *data, unsigned len, SocketAddress &info, int timeout) override;
	unsigned send(Socket &s, const void *data, unsigned len) override;
	unsigned trySend(Socket &s, const void *data, unsigned len, int timeout) override;
	unsigned sendto(Socket &s, const void *data, unsigned len, const SocketAddress &info) override;
	unsigned trySendto(Socket &s, const void *data, unsigned len, const SocketAddress &info, int timeout) override;
};

/**
 * @class Socket
 * @brief socket abstraction
 *
 * This class is a big wrapper around sockets functions but portable,
 * there is some functions that helps for getting error reporting.
 *
 * This class is implemented as a PIMPL idiom, it is perfectly
 * safe to cast the object to any other derivate children.
 */
class MALIKANIA_EXPORT Socket {
public:
#if defined(_WIN32)
	using Handle	= SOCKET;
	using ConstArg	= const char *;
	using Arg	= char *;
#else
	using Handle	= int;
	using ConstArg	= const void *;
	using Arg	= void *;
#endif

	using Iface	= std::shared_ptr<SocketInterface>;

protected:
	Iface	m_interface;				//!< the interface
	Handle	m_handle{INVALID_SOCKET};		//!< the socket shared pointer

public:
	/**
	 * To be called before any socket operation.
	 */
	static void init();

	/**
	 * Get the last socket system error. The error is set from errno or from
	 * WSAGetLastError on Windows.
	 *
	 * @return a string message
	 */
	static std::string syserror();

	/**
	 * Get the last system error.
	 *
	 * @param errn the error number (errno or WSAGetLastError)
	 * @return the error
	 */
	static std::string syserror(int errn);

	/**
	 * To be called before exiting.
	 */
	static void finish();

	/**
	 * Default constructor.
	 */
	Socket();

	/**
	 * Constructor to create a new socket.
	 *
	 * @param domain the domain
	 * @param type the type
	 * @param protocol the protocol
	 * @throw error::Failure on error
	 */
	Socket(int domain, int type, int protocol);

	/**
	 * Create a socket object with a already initialized socket.
	 *
	 * @param handle the handle
	 * @param interface the interface to use
	 */
	Socket(Handle handle, std::shared_ptr<SocketInterface> iface);

	/**
	 * Close the socket.
	 */
	virtual ~Socket() = default;

	/**
	 * Get the socket.
	 *
	 * @return the socket
	 */
	Handle handle() const;

	/**
	 * Set an option for the socket.
	 *
	 * @param level the setting level
	 * @param name the name
	 * @param arg the value
	 * @throw error::Failure on error
	 */
	template <typename Argument>
	void set(int level, int name, const Argument &arg)
	{
		if (setsockopt(m_handle, level, name, (Socket::ConstArg)&arg, sizeof (arg)) == SOCKET_ERROR)
			throw error::Error("set", syserror(), errno);
	}

	/**
	 * Get an option for the socket.
	 *
	 * @param level the setting level
	 * @param name the name
	 * @throw error::Failure on error
	 */
	template <typename Argument>
	Argument get(int level, int name)
	{
		Argument desired, result{};
		socklen_t size = sizeof (result);

		if (getsockopt(m_handle, level, name, (Socket::Arg)&desired, &size) == SOCKET_ERROR)
			throw error::Error("get", syserror(), errno);

		std::memcpy(&result, &desired, size);

		return result;
	}

	/**
	 * Enable or disable blocking mode.
	 *
	 * @param block the mode
	 * @throw error::Failure on error
	 */
	void blockMode(bool block = true);

	/**
	 * @copydoc SocketInterface::bind
	 */
	inline void bind(const SocketAddress &address)
	{
		m_interface->bind(*this, address);
	}

	/**
	 * @copydoc SocketInterface::close
	 */
	inline void close()
	{
		m_interface->close(*this);
	}

	/**
	 * @copydoc SocketInterface::connect
	 */
	inline void connect(const SocketAddress &address)
	{
		m_interface->connect(*this, address);
	}

	/**
	 * @copydoc SocketInterface::tryConnect
	 */
	inline void tryConnect(const SocketAddress &address, int timeout)
	{
		m_interface->tryConnect(*this, address, timeout);
	}

	/**
	 * @copydoc SocketInterface::tryAccept
	 */
	inline Socket tryAccept(SocketAddress &address, int timeout)
	{
		return m_interface->tryAccept(*this, address, timeout);
	}

	/**
	 * Overload without client information.
	 *
	 * @param timeout the timeout
	 * @return the socket
	 * @throw error::Error on errors
	 * @throw error::Timeout if operation has timeout
	 */
	Socket tryAccept(int timeout);

	/**
	 * Accept a client without getting its info.
	 *
	 * @return a client ready to use
	 * @throw error::Failure on error
	 */
	Socket accept();

	/**
	 * @copydoc SocketInterface::accept
	 */
	inline Socket accept(SocketAddress &info)
	{
		return m_interface->accept(*this, info);
	}

	/**
	 * @copydoc SocketInterface::listen
	 */
	inline void listen(int max)
	{
		m_interface->listen(*this, max);
	}

	/**
	 * @copydoc SocketInterface::recv
	 */
	inline unsigned recv(void *data, unsigned dataLen)
	{
		return m_interface->recv(*this, data, dataLen);
	}

	/**
	 * Overload for strings.
	 *
	 * @param count number of bytes to recv
	 * @return the string
	 * @throw error::Error on failures
	 * @throw error::WouldBlock if operation would block
	 */
	inline std::string recv(unsigned count)
	{
		std::string result;

		result.resize(count);
		auto n = recv(const_cast<char *>(result.data()), count);
		result.resize(n);

		return result;
	}

	/**
	 * @copydoc SocketInterface::tryRecv
	 */
	inline unsigned tryRecv(void *data, unsigned dataLen, int timeout = 0)
	{
		return m_interface->tryRecv(*this, data, dataLen, timeout);
	}

	/**
	 * Overload for string.
	 *
	 * @param count
	 * @param timeout
	 * @return the string
	 * @throw error::Error on failures
	 * @throw error::Timeout if operation has timeout
	 */
	inline std::string tryRecv(unsigned count, int timeout = 0)
	{
		std::string result;

		result.resize(count);
		auto n = tryRecv(const_cast<char *>(result.data()), count, timeout);
		result.resize(n);

		return result;
	}

	/**
	 * Receive from a connection-less socket without getting
	 * client information.
	 *
	 * @param data the destination pointer
	 * @param dataLen max length to receive
	 * @return the number of bytes received
	 * @throw error::Failure on error
	 */
	unsigned recvfrom(void *data, unsigned dataLen);

	/**
	 * @copydoc SocketInterface::recvfrom
	 */
	inline unsigned recvfrom(void *data, unsigned dataLen, SocketAddress &info)
	{
		return m_interface->recvfrom(*this, data, dataLen, info);
	}

	/**
	 * Overload for string.
	 *
	 * @param count the number of bytes to receive
	 * @return the string
	 * @throw error::Error on failures
	 * @throw error::WouldBlock if operation would block
	 */
	std::string recvfrom(unsigned count);

	/**
	 * Overload with client information.
	 *
	 * @param count the number of bytes to receive
	 * @return the string
	 * @throw error::Error on failures
	 * @throw error::WouldBlock if operation would block
	 */
	inline std::string recvfrom(unsigned count, SocketAddress &info)
	{
		std::string result;

		result.resize(count);
		auto n = recvfrom(const_cast<char *>(result.data()), count, info);
		result.resize(n);

		return result;
	}

	/**
	 * @copydoc SocketInterface::send
	 */
	inline unsigned send(const void *data, unsigned dataLen)
	{
		return m_interface->send(*this, data, dataLen);
	}

	/**
	 * Send a message as a string.
	 *
	 * @param message the message
	 * @return the number of bytes sent
	 * @throw SocketError on error
	 */
	inline unsigned send(const std::string &message)
	{
		return send(message.data(), message.size());
	}

	/**
	 * @copydoc SocketInterface::sendto
	 */
	inline unsigned sendto(const void *data, unsigned dataLen, const SocketAddress &info)
	{
		return m_interface->sendto(*this, data, dataLen, info);
	}

	/**
	 * Send a message to a connection-less socket.
	 *
	 * @param message the message
	 * @param address the address
	 * @return the number of bytes sent
	 * @throw SocketError on error
	 */
	inline unsigned sendto(const std::string &message, const SocketAddress &info)
	{
		return sendto(message.data(), message.size(), info);
	}
};

MALIKANIA_EXPORT bool operator==(const Socket &s1, const Socket &s2);

MALIKANIA_EXPORT bool operator<(const Socket &s, const Socket &s2);

} // !malikania

#endif // !_SOCKET_NG_H_

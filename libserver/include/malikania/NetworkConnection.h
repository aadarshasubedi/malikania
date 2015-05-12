/*
 * NetworkConnection.h -- base network connection for a socket
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

#ifndef _MALIKANIA_NETWORK_CONNECTION_H_
#define _MALIKANIA_NETWORK_CONNECTION_H_

/**
 * @file NetworkConnection.h
 * @brief Base level for network clients
 */

#include <string>
#include <vector>

#include <malikania/NetworkUtil.h>
#include <malikania/SocketSsl.h>
#include <malikania/SocketTcp.h>

namespace malikania {

/**
 * @class NetworkConnectionBase
 * @brief Base class for network clients
 *
 * This class provide a I/O buffer for only one socket so the player
 * have two instance of this class, one for SSL, one for clear channel.
 */
template <typename Sock>
class NetworkConnectionBase {
private:
	Sock m_socket;
	std::string m_input;
	std::string m_output;

public:
	/**
	 * Create a network connection with the specified socket.
	 *
	 * @warning the socket must be marked non blocking
	 * @param sc the socket
	 */
	NetworkConnectionBase(Sock sc)
		: m_socket(std::move(sc))
	{
	}

	/**
	 * Get the underlying socket.
	 *
	 * @return the socket
	 */
	inline Sock &socket() noexcept
	{
		return m_socket;
	}

	/**
	 * Append some data to the socket, this does not send it it just
	 * append it to the queue.
	 *
	 * @note You must not append \r\n\r\n
	 * @param s the data
	 */
	inline void append(std::string s)
	{
		m_output += std::move(s);
		m_output += "\r\n\r\n";
	}

	/**
	 * Tells if some data is pending for output.
	 *
	 * @return true if the output buffer is not empty
	 */
	inline bool hasOutput() const noexcept
	{
		return !m_output.empty();
	}

	/**
	 * Read some data into the client.
	 *
	 * @throw SocketError if any error occurs
	 */
	inline void read()
	{
		m_input += m_socket.recv(512);
	}

	/**
	 * Send the most data possible to the client.
	 *
	 * @throw SocketError if any error occurs
	 */
	inline void send()
	{
		m_output.erase(0, m_socket.send(m_output));
	}

	/**
	 * Empty the input buffer and convert it as a queue of messages.
	 *
	 * @return the messages or an empty list if no message is available
	 */
	inline std::vector<std::string> data()
	{
		return NetworkUtil::split(m_input);
	}
};

/**
 * Helper for clear sockets.
 */
using NetworkConnection = NetworkConnectionBase<SocketTcp>;

/**
 * Helper for SSL sockets.
 */
using NetworkConnectionSsl = NetworkConnectionBase<SocketSsl>;

} // !malikania

#endif // !_MALIKANIA_NETWORK_CONNECTION_H_

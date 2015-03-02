/*
 * SocketListener.h -- portable select() wrapper
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

#ifndef _SOCKET_LISTENER_H_
#define _SOCKET_LISTENER_H_

#include <chrono>
#include <functional>
#include <initializer_list>
#include <utility>
#include <vector>

#include "Socket.h"

#if defined(_WIN32)
#  if _WIN32_WINNT >= 0x0600
#    define SOCKET_LISTENER_HAVE_POLL
#  endif
#else
#  define SOCKET_LISTENER_HAVE_POLL
#endif

namespace malikania {

/**
 * @enum SocketMethod
 * @brief The SocketMethod enum
 *
 * Select the method of polling. It is only a preferred method, for example if you
 * request for poll but it is not available, select will be used.
 */
enum SocketMethod {
	Select,				//!< select(2) method, fallback
	Poll				//!< poll(2), everywhere possible
};

/**
 * @struct SocketStatus
 * @brief The SocketStatus struct
 *
 * Result of a select call, returns the first ready socket found with its
 * direction.
 */
struct SocketStatus {
	Socket	socket;			//!< which socket is ready
	int	direction;		//!< the direction
};

/**
 * @class SocketListener
 * @brief Synchronous multiplexing
 *
 * Convenient wrapper around the select() system call.
 */
class MALIKANIA_EXPORT SocketListener final {
public:
	/**
	 * @brief Function for listing all sockets
	 */
	using MapFunc = std::function<void (Socket &, int)>;

#if defined(SOCKET_LISTENER_HAVE_POLL)
	static constexpr const SocketMethod PreferredMethod = SocketMethod::Poll;
#else
	static constexpr const SocketMethod PreferredMethod = SocketMethod::Select;
#endif

	/**
	 * @class Interface
	 * @brief Implement the polling method
	 */
	class Interface {
	public:
		/**
		 * Default destructor.
		 */
		virtual ~Interface() = default;

		/**
		 * List all sockets in the interface.
		 *
		 * @param func the function
		 */
		virtual void list(const MapFunc &func) = 0;

		/**
		 * Add a socket with a specified direction.
		 *
		 * @param s the socket
		 * @param direction the direction
		 */
		virtual void add(Socket s, int direction) = 0;

		/**
		 * Remove a socket with a specified direction.
		 *
		 * @param s the socket
		 * @param direction the direction
		 */
		virtual void remove(const Socket &s, int direction) = 0;

		/**
		 * Remove all sockets.
		 */
		virtual void clear() = 0;

		/**
		 * Get the total number of sockets in the listener.
		 */
		virtual unsigned size() const = 0;

		/**
		 * Select one socket.
		 *
		 * @param ms the number of milliseconds to wait, -1 means forever
		 * @return the socket status
		 * @throw error::Failure on failure
		 * @throw error::Timeout on timeout
		 */
		virtual SocketStatus select(int ms) = 0;

		/**
		 * Select many sockets.
		 *
		 * @param ms the number of milliseconds to wait, -1 means forever
		 * @return a vector of ready sockets
		 * @throw error::Failure on failure
		 * @throw error::Timeout on timeout
		 */
		virtual std::vector<SocketStatus> selectMultiple(int ms) = 0;
	};

	std::unique_ptr<Interface> m_interface;

public:
	/**
	 * Move constructor.
	 *
	 * @param other the other object
	 */
	SocketListener(SocketListener &&other) = default;

	/**
	 * Move operator.
	 *
	 * @param other the other object
	 * @return this
	 */
	SocketListener &operator=(SocketListener &&other) = default;

	/**
	 * Create a socket listener.
	 *
	 * @param method the preferred method
	 */
	SocketListener(int method = PreferredMethod);

	/**
	 * Createa listener with some sockets.
	 *
	 * @param list the initializer list
	 * @param method the preferred method
	 */
	SocketListener(std::initializer_list<std::pair<Socket, int>> list, int method = PreferredMethod);

	/**
	 * Add a socket to listen to.
	 *
	 * @param s the socket
	 * @param direction the direction
	 */
	inline void add(Socket s, int direction)
	{
		m_interface->add(std::move(s), direction);
	}

	/**
	 * Remove a socket from the list.
	 *
	 * @param s the socket
	 * @param direction the direction
	 */
	inline void remove(const Socket &s, int direction)
	{
		m_interface->remove(s, direction);
	}

	/**
	 * Remove every sockets in the listener.
	 */
	inline void clear()
	{
		m_interface->clear();
	}

	/**
	 * Get the number of clients in listener.
	 *
	 * @return the total number of sockets in the listener
	 */
	inline unsigned size() const
	{
		return m_interface->size();
	}

	/**
	 * Select a socket. Waits for a specific amount of time specified as the duration.
	 *
	 * @param duration the duration
	 * @return the socket ready
	 * @throw SocketError on error
	 * @throw SocketTimeout on timeout
	 */
	template <typename Rep, typename Ratio>
	inline SocketStatus select(const std::chrono::duration<Rep, Ratio> &duration)
	{
		auto cvt = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		return m_interface->select(cvt.count());
	}

	/**
	 * Overload with milliseconds.
	 *
	 * @param timeout the optional timeout in milliseconds
	 * @return the socket ready
	 * @throw SocketError on error
	 * @throw SocketTimeout on timeout
	 */
	inline SocketStatus select(int timeout = -1)
	{
		return m_interface->select(timeout);
	}

	/**
	 * Select multiple sockets.
	 *
	 * @param duration the duration
	 * @return the socket ready
	 * @throw SocketError on error
	 * @throw SocketTimeout on timeout
	 */
	template <typename Rep, typename Ratio>
	inline std::vector<SocketStatus> selectMultiple(const std::chrono::duration<Rep, Ratio> &duration)
	{
		auto cvt = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		return m_interface->selectMultiple(cvt.count());
	}

	/**
	 * Overload with milliseconds.
	 *
	 * @return the socket ready
	 * @throw SocketError on error
	 * @throw SocketTimeout on timeout
	 */
	inline std::vector<SocketStatus> selectMultiple(int timeout = -1)
	{
		return m_interface->selectMultiple(timeout);
	}

	/**
	 * List every socket in the listener.
	 *
	 * @param func the function to call
	 */
	template <typename Func>
	inline void list(Func func)
	{
		m_interface->list(func);
	}
};

} // !malikania

#endif // !_SOCKET_LISTENER_H_

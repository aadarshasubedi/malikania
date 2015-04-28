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

#ifndef _SOCKET_LISTENER_NG_H_
#define _SOCKET_LISTENER_NG_H_

/**
 * Feature detection, multiple implementations may be avaible, for example,
 * Linux has poll, select and epoll.
 *
 * We assume that select(2) is always available.
 *
 * Of course, you can set the variables yourself if you test it with your
 * build system.
 *
 * SOCKET_HAVE_POLL	- Defined on all BSD, Linux. Also defined on Windows
 *			  if _WIN32_WINNT is set to 0x0600 or greater.
 * SOCKET_HAVE_KQUEUE	- Defined on all BSD and Apple.
 * SOCKET_HAVE_EPOLL	- Defined on Linux only.
 */
#if defined(_WIN32)
#  if _WIN32_WINNT >= 0x0600
#    define SOCKET_HAVE_POLL
#  endif
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
#  define SOCKET_HAVE_KQUEUE
#  define SOCKET_HAVE_POLL
#elif defined(__linux__)
#  define SOCKET_HAVE_EPOLL
#  define SOCKET_HAVE_POLL
#endif

/**
 * This sets the default backend to use depending on the system. The following
 * table summaries.
 *
 * The preference priority is ordered from left to right.
 *
 * +---------------+-------------------------+
 * | System        | Backend                 |
 * +---------------+-------------------------+
 * | Linux         | epoll(2)                |
 * | *BSD          | kqueue(2)               |
 * | Windows       | poll(2), select(2)      |
 * | Mac OS X      | kqueue(2)               |
 * +---------------+-------------------------+
 */

#if defined(_WIN32)
#  if defined(SOCKET_HAVE_POLL)
#    define SOCKET_DEFAULT_BACKEND backend::Poll
#  else
#    define SOCKET_DEFAULT_BACKEND backend::Select
#  endif
#elif defined(__linux__)
   // TODO NOT READY YET
   //#  define SOCKET_DEFAULT_BACKEND backend::Epoll
#  define SOCKET_DEFAULT_BACKEND backend::Poll
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
#  include <sys/stat.h>
#  include <sys/event.h>
#  include <sys/time.h>

#  define SOCKET_DEFAULT_BACKEND backend::Kqueue
#else
#  define SOCKET_DEFAULT_BACKEND backend::Select
#endif

#include <chrono>
#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "Socket.h"

#if defined(SOCKET_HAVE_POLL) && !defined(_WIN32)
#  include <poll.h>
#endif

/**
 * @struct SocketStatus
 * @brief The SocketStatus class
 *
 * Result of a select call, returns the first ready socket found with its
 * flags.
 */
class SocketStatus {
public:
	Socket socket;		//!< which socket is ready
	int flags;		//!< the flags
};

namespace backend {

/**
 * @class Select
 * @brief Implements select(2)
 *
 * This class is the fallback of any other method, it is not preferred at all for many reasons.
 */
class Select {
private:
	std::map<Socket::Handle, std::pair<Socket, int>> m_table;

public:
	void set(Socket s, int flags);
	void unset(Socket s, int flags);
	void remove(Socket sc);
	void clear();
	SocketStatus wait(int ms);
	std::vector<SocketStatus> waitMultiple(int ms);
};

#if defined(SOCKET_HAVE_POLL)

/**
 * @class Poll
 * @brief Implements poll(2)
 *
 * Poll is widely supported and is better than select(2). It is still not the
 * best option as selecting the sockets is O(n).
 */
class Poll {
private:
	std::vector<pollfd> m_fds;
	std::map<Socket::Handle, Socket> m_lookup;

	short topoll(int flags) const noexcept;
	int toflags(short &event) const noexcept;

public:
	void set(Socket s, int flags);
	void unset(Socket s, int flags);
	void remove(Socket sc);
	void clear();
	SocketStatus wait(int ms);
	std::vector<SocketStatus> waitMultiple(int ms);
};

#endif

#if defined(SOCKET_HAVE_EPOLL)

class Epoll {
	// TODO
};

#endif

#if defined(SOCKET_HAVE_KQUEUE)

/**
 * @class Kqueue
 * @brief Implements kqueue(2)
 *
 * This implementation is available on all BSD and Mac OS X. It is better than
 * poll(2) because it's O(1), however it's a bit more memory consuming.
 */
class Kqueue {
private:
	std::map<Socket::Handle, std::pair<Socket, int>> m_table;
	std::vector<struct kevent> m_result;
	int m_handle;

	Kqueue(const Kqueue &) = delete;
	Kqueue &operator=(const Kqueue &) = delete;
	Kqueue(Kqueue &&) = delete;
	Kqueue &operator=(Kqueue &&) = delete;

	void update(Socket &sc, int filter, int flags);

public:
	Kqueue();
	~Kqueue();

	void set(Socket sc, int flags);
	void unset(Socket sc, int flags);
	void remove(Socket sc);
	void clear();
	SocketStatus wait(int ms);
	std::vector<SocketStatus> waitMultiple(int ms);
};

#endif

} // !backend

/**
 * @class SocketListenerBase
 * @brief Synchronous multiplexing
 *
 * Convenient wrapper around the select() system call.
 *
 * This class is implemented using a bridge pattern to allow different uses
 * of listener implementation.
 *
 * You should not reinstanciate a new SocketListener at each iteartion of your
 * main loop as it can be extremely costly. Instead use the same listener that
 * you can safely modify on the fly.
 *
 * Currently, poll, select and kqueue are available.
 */
template <typename Backend = SOCKET_DEFAULT_BACKEND>
class SocketListenerBase final {
public:
	static const int Read;
	static const int Write;

	using Map = std::map<Socket, int>;

private:
	Backend m_backend;
	Map m_map;

public:
	inline SocketListenerBase()
	{
	}

	/**
	 * Create a listener from a list of sockets.
	 *
	 * @param list the list
	 */
	inline SocketListenerBase(std::initializer_list<std::pair<Socket, int>> list)
	{
		for (const auto &p : list) {
			set(p.first, p.second);
		}
	}

	/**
	 * Return an iterator to the beginning.
	 *
	 * @return the iterator
	 */
	inline auto begin() noexcept
	{
		return m_map.begin();
	}

	/**
	 * Overloaded function.
	 *
	 * @return the iterator
	 */
	inline auto begin() const noexcept
	{
		return m_map.begin();
	}

	/**
	 * Overloaded function.
	 *
	 * @return the iterator
	 */
	inline auto cbegin() const noexcept
	{
		return m_map.cbegin();
	}

	/**
	 * Return an iterator to the end.
	 *
	 * @return the iterator
	 */
	inline auto end() noexcept
	{
		return m_map.end();
	}

	/**
	 * Overloaded function.
	 *
	 * @return the iterator
	 */
	inline auto end() const noexcept
	{
		return m_map.end();
	}

	/**
	 * Overloaded function.
	 *
	 * @return the iterator
	 */
	inline auto cend() const noexcept
	{
		return m_map.cend();
	}

	/**
	 * Add a socket to the listener.
	 *
	 * @param sc the socket
	 * @param flags (may be OR'ed)
	 */
	void set(Socket sc, int flags)
	{
		if (m_map.count(sc) > 0) {
			m_map[sc] |= flags;
			m_backend.set(sc, flags);
		} else {
			m_map.insert({sc, flags});
			m_backend.set(sc, flags);
		}
	}

	/**
	 * Unset a socket from the listener, only the flags is removed
	 * unless the two flagss are requested.
	 *
	 * For example, if you added a socket for both reading and writing,
	 * unsetting the write flags will keep the socket for reading.
	 *
	 * @param sc the socket
	 * @param flags the flags (may be OR'ed)
	 * @see remove
	 */
	void unset(Socket sc, int flags) noexcept
	{
		if (m_map.count(sc) > 0) {
			m_map[sc] &= ~(flags);
			m_backend.unset(sc, flags);

			// No more flags, remove it
			if (m_map[sc] == 0) {
				m_map.erase(sc);
			}
		}
	}

	/**
	 * Remove completely the socket from the listener.
	 *
	 * @param sc the socket
	 */
	inline void remove(Socket sc) noexcept
	{
		m_map.erase(sc);
		m_backend.remove(sc);
	}

	/**
	 * Remove all sockets.
	 */
	inline void clear() noexcept
	{
		m_map.clear();
		m_backend.clear();
	}

	/**
	 * Get the number of sockets in the listener.
	 */
	unsigned size() const noexcept
	{
		return m_map.size();
	}

	/**
	 * Select a socket. Waits for a specific amount of time specified as the duration.
	 *
	 * @param duration the duration
	 * @return the socket ready
	 */
	template <typename Rep, typename Ratio>
	inline SocketStatus wait(const std::chrono::duration<Rep, Ratio> &duration)
	{
		auto cvt = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		return m_backend.wait(cvt.count());
	}

	/**
	 * Overload with milliseconds.
	 *
	 * @param timeout the optional timeout in milliseconds
	 * @return the socket ready
	 */
	inline SocketStatus wait(int timeout = -1)
	{
		return m_backend.wait(timeout);
	}

	/**
	 * Select multiple sockets.
	 *
	 * @param duration the duration
	 * @return the socket ready
	 */
	template <typename Rep, typename Ratio>
	inline std::vector<SocketStatus> waitMultiple(const std::chrono::duration<Rep, Ratio> &duration)
	{
		auto cvt = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		return m_backend.waitMultiple(cvt.count());
	}

	/**
	 * Overload with milliseconds.
	 *
	 * @return the socket ready
	 */
	inline std::vector<SocketStatus> waitMultiple(int timeout = -1)
	{
		return m_backend.waitMultiple(timeout);
	}
};

/**
 * Helper to use the default.
 */
using SocketListener = SocketListenerBase<>;

template <typename Backend>
const int SocketListenerBase<Backend>::Read{1 << 0};

template <typename Backend>
const int SocketListenerBase<Backend>::Write{1 << 1};

#endif // !_SOCKET_LISTENER_NG_H_

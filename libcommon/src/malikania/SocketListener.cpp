/*
 * SocketListener.cpp -- portable select() wrapper
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

#include <algorithm>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "SocketListener.h"

/* --------------------------------------------------------
 * Select implementation
 * -------------------------------------------------------- */

namespace backend {

void Select::set(Socket s, int direction)
{
	if (m_table.count(s.handle()) > 0) {
		m_table.at(s.handle()).second |= direction;
	} else {
		m_table.insert({s.handle(), {s, direction}});
	}

}

void Select::unset(Socket s, int direction)
{
	if (m_table.count(s.handle()) != 0) {
		m_table.at(s.handle()).second &= ~(direction);

		// If no read, no write is requested, remove it
		if (m_table.at(s.handle()).second == 0) {
			m_table.erase(s.handle());
		}
	}
}

void Select::remove(Socket sc)
{
	m_table.erase(sc.handle());
}

void Select::clear()
{
	m_table.clear();
}

SocketStatus Select::wait(int ms)
{
	auto result = waitMultiple(ms);

	if (result.size() == 0) {
		throw SocketError(SocketError::System, "select", "No socket found");
	}

	return result[0];
}

std::vector<SocketStatus> Select::waitMultiple(int ms)
{
	timeval maxwait, *towait;
	fd_set readset;
	fd_set writeset;

	FD_ZERO(&readset);
	FD_ZERO(&writeset);

	Socket::Handle max = 0;

	for (auto &s : m_table) {
		if (s.second.second & SocketListener::Read) {
			FD_SET(s.first, &readset);
		}
		if (s.second.second & SocketListener::Write) {
			FD_SET(s.first, &writeset);
		}

		if (s.first > max) {
			max = s.first;
		}
	}

	maxwait.tv_sec = 0;
	maxwait.tv_usec = ms * 1000;

	// Set to nullptr for infinite timeout.
	towait = (ms < 0) ? nullptr : &maxwait;

	auto error = ::select(max + 1, &readset, &writeset, nullptr, towait);
	if (error == Socket::Error) {
		throw SocketError(SocketError::System, "select");
	}
	if (error == 0) {
		throw SocketError(SocketError::Timeout, "select", "Timeout while listening");
	}

	std::vector<SocketStatus> sockets;

	for (auto &c : m_table) {
		if (FD_ISSET(c.first, &readset)) {
			sockets.push_back({ c.second.first, SocketListener::Read });
		}
		if (FD_ISSET(c.first, &writeset)) {
			sockets.push_back({ c.second.first, SocketListener::Write });
		}
	}

	return sockets;
}

/* --------------------------------------------------------
 * Poll implementation
 * -------------------------------------------------------- */

#if defined(SOCKET_HAVE_POLL)

#if defined(_WIN32)
#  define poll WSAPoll
#endif

short Poll::topoll(int direction) const noexcept
{
	short result(0);

	if (direction & SocketListener::Read) {
		result |= POLLIN;
	}
	if (direction & SocketListener::Write) {
		result |= POLLOUT;
	}

	return result;
}

int Poll::todirection(short event) const noexcept
{
	int direction = 0;

	/*
	 * Poll implementations mark the socket differently regarding
	 * the disconnection of a socket.
	 *
	 * At least, even if POLLHUP or POLLIN is set, recv() always
	 * return 0 so we mark the socket as readable.
	 */
	if ((event & POLLIN) || (event & POLLHUP)) {
		direction |= SocketListener::Read;
	}
	if (event & POLLOUT) {
		direction |= SocketListener::Write;
	}

	return direction;
}

void Poll::set(Socket s, int direction)
{
	auto it = std::find_if(m_fds.begin(), m_fds.end(), [&] (const auto &pfd) { return pfd.fd == s.handle(); });

	// If found, add the new direction, otherwise add a new socket
	if (it != m_fds.end()) {
		it->events |= topoll(direction);
	} else {
		m_lookup.insert({s.handle(), s});
		m_fds.push_back({ s.handle(), topoll(direction), 0 });
	}
}

void Poll::unset(Socket s, int direction)
{
	for (auto i = m_fds.begin(); i != m_fds.end();) {
		if (i->fd == s.handle()) {
			i->events &= ~(topoll(direction));

			if (i->events == 0) {
				m_lookup.erase(i->fd);
				i = m_fds.erase(i);
			} else {
				++i;
			}
		} else {
			++i;
		}
	}
}

void Poll::remove(Socket s)
{
	auto it = std::find_if(m_fds.begin(), m_fds.end(), [&] (const auto &pfd) { return pfd.fd == s.handle(); });

	if (it != m_fds.end()) {
		m_fds.erase(it);
		m_lookup.erase(s.handle());
	}
}

void Poll::clear()
{
	m_fds.clear();
	m_lookup.clear();
}

SocketStatus Poll::wait(int ms)
{
	auto result = poll(m_fds.data(), m_fds.size(), ms);
	if (result == 0) {
		throw SocketError(SocketError::Timeout, "select", "Timeout while listening");
	}
	if (result < 0) {
		throw SocketError(SocketError::System, "poll");
	}

	for (auto &fd : m_fds) {
		if (fd.revents != 0) {
			return { m_lookup.at(fd.fd), todirection(fd.revents) };
		}
	}

	throw SocketError(SocketError::System, "select", "No socket found");
}

std::vector<SocketStatus> Poll::waitMultiple(int ms)
{
	auto result = poll(m_fds.data(), m_fds.size(), ms);
	if (result == 0) {
		throw SocketError(SocketError::Timeout, "select", "Timeout while listening");
	}
	if (result < 0) {
		throw SocketError(SocketError::System, "poll");
	}

	std::vector<SocketStatus> sockets;
	for (auto &fd : m_fds) {
		if (fd.revents != 0) {
			sockets.push_back({ m_lookup.at(fd.fd), todirection(fd.revents) });
		}
	}

	return sockets;
}

#endif // !SOCKET_HAVE_POLL

/* --------------------------------------------------------
 * Kqueue implementation
 * -------------------------------------------------------- */

#if defined(SOCKET_HAVE_KQUEUE)

Kqueue::Kqueue()
	: m_handle(nullptr, nullptr)
{
	int handle = kqueue();

	if (handle < 0) {
		throw SocketError(SocketError::System, "kqueue");
	}

	m_handle = std::unique_ptr<int, void (*)(int *)>(new int(handle), [] (int *p) {
		(void)::close(*p);
	});
}

std::vector<struct kevent>::iterator Kqueue::find(const Socket &s)
{
	return std::find_if(m_list.begin(), m_list.end(), [&] (struct kevent &kv) -> bool {
		return static_cast<Socket::Handle>(kv.ident) == s.handle();
	});
}

void Kqueue::set(Socket &s, int direction)
{
	struct kevent ev;

	if (direction == SocketListener::Read) {
		EV_SET(&ev, s.handle(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &s);
	} else if (direction == SocketListener::Write) {
		EV_SET(&ev, s.handle(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &s);
	}

	auto it = find(s);

	if (it == m_list.end()) {
		m_list.push_back(ev);
	} else {
		*it = ev;
	}

	m_result.resize(m_list.size());
}

void Kqueue::unset(Socket s, int direction)
{
	auto it = find(s);

	if (it != m_list.end()) {
		if (direction & SocketListener::Read) {
			it->filter &= ~(EVFILT_READ);
		}
		if (direction & SocketListener::Write) {
			it->filter &= ~(EVFILT_WRITE);
		}

		/* complete removal */
		if ((it->filter & ~(direction)) == 0) {
			m_list.erase(it);
		}
	}
}

void Kqueue::remove(Socket s)
{
	auto it = find(s);

	if (it != m_list.end()) {
		m_list.erase(it);
	}

	m_result.resize(m_list.size());
}

void Kqueue::clear()
{
	m_list.clear();
	m_result.clear();

	m_list.resize(0);
	m_result.resize(0);
}

SocketStatus Kqueue::wait(int ms)
{
	return waitMultiple(ms)[0];
}

std::vector<SocketStatus> Kqueue::waitMultiple(int ms)
{
	std::vector<SocketStatus> sockets;
	timespec ts = { 0, 0 };
	timespec *pts = (ms <= 0) ? nullptr : &ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;

	int nevents = kevent(*m_handle, m_list.data(), m_list.size(), &m_result[0], m_result.capacity(), pts);

	if (nevents == 0) {
		throw SocketError(SocketError::Timeout, "kevent");
	}
	if (nevents < 0) {
		throw SocketError(SocketError::System, "kevent");
	}

	for (int i = 0; i < nevents; ++i) {
		sockets.push_back(SocketStatus{
			*static_cast<Socket *>(m_result[i].udata),
			(m_result[i].filter & EVFILT_READ) ? SocketListener::Read : SocketListener::Write
		});
	}

	return sockets;
}

#endif // !SOCKET_HAVE_KQUEUE

} // !backend

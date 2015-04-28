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
#include <set>

#include "SocketListener.h"

/* --------------------------------------------------------
 * Select implementation
 * -------------------------------------------------------- */

namespace backend {

void Select::set(Socket s, int flags)
{
	if (m_table.count(s.handle()) > 0) {
		m_table.at(s.handle()).second |= flags;
	} else {
		m_table.emplace(s.handle(), std::make_pair(s, flags));
	}
}

void Select::unset(Socket s, int flags)
{
	if (m_table.count(s.handle()) != 0) {
		m_table.at(s.handle()).second &= ~(flags);

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
	std::vector<SocketStatus> result = waitMultiple(ms);

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
			sockets.push_back(SocketStatus{c.second.first, SocketListener::Read});
		}
		if (FD_ISSET(c.first, &writeset)) {
			sockets.push_back(SocketStatus{c.second.first, SocketListener::Write});
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

short Poll::topoll(int flags) const noexcept
{
	short result(0);

	if (flags & SocketListener::Read) {
		result |= POLLIN;
	}
	if (flags & SocketListener::Write) {
		result |= POLLOUT;
	}

	return result;
}

int Poll::toflags(short &event) const noexcept
{
	int flags = 0;

	/*
	 * Poll implementations mark the socket differently regarding
	 * the disconnection of a socket.
	 *
	 * At least, even if POLLHUP or POLLIN is set, recv() always
	 * return 0 so we mark the socket as readable.
	 */
	if ((event & POLLIN) || (event & POLLHUP)) {
		flags |= SocketListener::Read;
	}
	if (event & POLLOUT) {
		flags |= SocketListener::Write;
	}

	// Reset event for safety
	event = 0;

	return flags;
}

void Poll::set(Socket s, int flags)
{
	auto it = std::find_if(m_fds.begin(), m_fds.end(), [&] (const auto &pfd) {
		return pfd.fd == s.handle();
	});

	// If found, add the new flags, otherwise add a new socket
	if (it != m_fds.end()) {
		it->events |= topoll(flags);
	} else {
		m_lookup.emplace(s.handle(), s);
		m_fds.push_back({ s.handle(), topoll(flags), 0 });
	}
}

void Poll::unset(Socket s, int flags)
{
	for (auto i = m_fds.begin(); i != m_fds.end();) {
		if (i->fd == s.handle()) {
			i->events &= ~(topoll(flags));

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
			return SocketStatus{m_lookup.at(fd.fd), toflags(fd.revents)};
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
			sockets.push_back(SocketStatus{m_lookup.at(fd.fd), toflags(fd.revents)});
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
	: m_handle(kqueue())
{
	if (m_handle < 0) {
		throw SocketError(SocketError::System, "kqueue");
	}
}

Kqueue::~Kqueue()
{
	close(m_handle);
}

void Kqueue::update(Socket &sc, int filter, int flags)
{
	struct kevent ev;

	EV_SET(&ev, sc.handle(), filter, flags, 0, 0, nullptr);

	if (kevent(m_handle, &ev, 1, nullptr, 0, nullptr) < 0) {
		throw SocketError(SocketError::System, "kevent");
	}
}

void Kqueue::set(Socket sc, int flags)
{
	if (flags & SocketListener::Read) {
		puts("About to set to add");
		update(sc, EVFILT_READ, EV_ADD | EV_ENABLE);
	}
	if (flags & SocketListener::Write) {
		puts("About to set to remove");
		update(sc, EVFILT_WRITE, EV_ADD | EV_ENABLE);
	}

	auto it = m_table.find(sc.handle());
	if (it == m_table.end()) {
		m_table.emplace(sc.handle(), std::make_pair(std::move(sc), flags));
	} else {
		it->second.second |= flags;
	}

	m_result.resize(m_table.size());
}

void Kqueue::unset(Socket sc, int flags)
{
	if (flags & SocketListener::Read) {
		update(sc, EVFILT_READ, EV_DELETE);
	}
	if (flags & SocketListener::Write) {
		update(sc, EVFILT_WRITE, EV_DELETE);
	}

	auto it = m_table.find(sc.handle());
	if (it != m_table.end()) {
		it->second.second &= ~(flags);

		if (it->second.second == 0) {
			m_table.erase(it);
		}
	}

	m_result.resize(m_table.size());
}

void Kqueue::remove(Socket sc)
{
	auto it = m_table.find(sc.handle());

	if (it != m_table.end()) {
		if (it->second.second & SocketListener::Read) {
			update(sc, EVFILT_READ, EV_DELETE);
		}
		if (it->second.second & SocketListener::Write) {
			update(sc, EVFILT_WRITE, EV_DELETE);
		}

		m_table.erase(sc.handle());
	}

	m_result.resize(m_table.size());
}

void Kqueue::clear()
{
	for (auto &pair : m_table) {
		update(pair.second.first, EVFILT_READ, EV_DELETE);
		update(pair.second.first, EVFILT_WRITE, EV_DELETE);
	}

	m_table.clear();
	m_result.resize(0U);
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

	int nevents = kevent(m_handle, nullptr, 0, &m_result[0], m_result.capacity(), pts);

	if (nevents == 0) {
		throw SocketError(SocketError::Timeout, "kevent");
	}
	if (nevents < 0) {
		throw SocketError(SocketError::System, "kevent");
	}

	for (int i = 0; i < nevents; ++i) {
		Socket sc = m_table.at(m_result[i].ident).first;
		int flags = m_result[i].filter == EVFILT_READ ? SocketListener::Read : SocketListener::Write;

		sockets.push_back(SocketStatus{sc, flags});
	}

	return sockets;
}

#endif // !SOCKET_HAVE_KQUEUE

} // !backend

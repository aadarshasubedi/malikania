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

namespace malikania {

using namespace direction;

/* --------------------------------------------------------
 * Select implementation
 * -------------------------------------------------------- */

/**
 * @class SelectMethod
 * @brief Implements select(2)
 *
 * This class is the fallback of any other method, it is not preferred at all for many reasons.
 */
class SelectMethod final : public SocketListener::Interface {
private:
	std::map<Socket::Handle, std::pair<Socket, int>> m_table;

public:
	void add(Socket s, int direction) override;
	void remove(const Socket &s, int direction) override;
	void list(const SocketListener::MapFunc &func) override;
	void clear() override;
	unsigned size() const override;
	SocketStatus select(int ms) override;
	std::vector<SocketStatus> selectMultiple(int ms) override;
};

void SelectMethod::add(Socket s, int direction)
{
	if (m_table.count(s.handle()) > 0)
		m_table[s.handle()].second |= direction;
	else
		m_table[s.handle()] = { s, direction };
}

void SelectMethod::remove(const Socket &s, int direction)
{
	if (m_table.count(s.handle()) != 0) {
		m_table[s.handle()].second &= ~(direction);

		// If no read, no write is requested, remove it
		if (m_table[s.handle()].second == 0)
			m_table.erase(s.handle());
	}
}

void SelectMethod::list(const SocketListener::MapFunc &func)
{
	for (auto &s : m_table)
		func(s.second.first, s.second.second);
}

void SelectMethod::clear()
{
	m_table.clear();
}

unsigned SelectMethod::size() const
{
	return m_table.size();
}

SocketStatus SelectMethod::select(int ms)
{
	auto result = selectMultiple(ms);

	if (result.size() == 0)
		throw error::Error("select", "No socket found", 0);

	return result[0];
}

std::vector<SocketStatus> SelectMethod::selectMultiple(int ms)
{
	timeval maxwait, *towait;
	fd_set readset;
	fd_set writeset;

	FD_ZERO(&readset);
	FD_ZERO(&writeset);

	Socket::Handle max = 0;

	for (auto &s : m_table) {
		if (s.second.second & Read)
			FD_SET(s.first, &readset);
		if (s.second.second & Write)
			FD_SET(s.first, &writeset);

		if (s.first > max)
			max = s.first;
	}

	maxwait.tv_sec = 0;
	maxwait.tv_usec = ms * 1000;

	// Set to nullptr for infinite timeout.
	towait = (ms < 0) ? nullptr : &maxwait;

	auto error = ::select(max + 1, &readset, &writeset, nullptr, towait);
	if (error == SOCKET_ERROR)
#if defined(_WIN32)
		throw error::Error("select", Socket::syserror(), WSAGetLastError());
#else
		throw error::Error("select", Socket::syserror(), errno);
#endif
	if (error == 0)
		throw error::Timeout("select");

	std::vector<SocketStatus> sockets;

	for (auto &c : m_table) {
		if (FD_ISSET(c.first, &readset))
			sockets.push_back({ c.second.first, Read });
		if (FD_ISSET(c.first, &writeset))
			sockets.push_back({ c.second.first, Write });
	}

	return sockets;
}

/* --------------------------------------------------------
 * Poll implementation
 * -------------------------------------------------------- */

#if defined(SOCKET_LISTENER_HAVE_POLL)

#if defined(_WIN32)
#  include <Winsock2.h>
#  define poll WSAPoll
#else
#  include <poll.h>
#endif

namespace {

class PollMethod final : public SocketListener::Interface {
private:
	std::vector<pollfd> m_fds;
	std::map<Socket::Handle, Socket> m_lookup;

	inline short topoll(int direction)
	{
		short result(0);

		if (direction & Read)
			result |= POLLIN;
		if (direction & Write)
			result |= POLLOUT;

		return result;
	}

	inline int todirection(short event)
	{
		int direction{};

		/*
		 * Poll implementations mark the socket differently regarding
		 * the disconnection of a socket.
		 *
		 * At least, even if POLLHUP or POLLIN is set, recv() always
		 * return 0 so we mark the socket as readable.
		 */
		if ((event & POLLIN) || (event & POLLHUP))
			direction |= Read;
		if (event & POLLOUT)
			direction |= Write;

		return direction;
	}

public:
	void add(Socket s, int direction) override;
	void remove(const Socket &s, int direction) override;
	void list(const SocketListener::MapFunc &func) override;
	void clear() override;
	unsigned size() const override;
	SocketStatus select(int ms) override;
	std::vector<SocketStatus> selectMultiple(int ms) override;
};

void PollMethod::add(Socket s, int direction)
{
	auto it = std::find_if(m_fds.begin(), m_fds.end(), [&] (const auto &pfd) { return pfd.fd == s.handle(); });

	// If found, add the new direction, otherwise add a new socket
	if (it != m_fds.end())
		it->events |= topoll(direction);
	else {
		m_lookup[s.handle()] = s;
		m_fds.push_back({ s.handle(), topoll(direction), 0 });
	}
}

void PollMethod::remove(const Socket &s, int direction)
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
		} else
			++i;
	}
}

void PollMethod::list(const SocketListener::MapFunc &func)
{
	for (auto &fd : m_fds)
		func(m_lookup[fd.fd], todirection(fd.events));
}

void PollMethod::clear()
{
	m_fds.clear();
	m_lookup.clear();
}

unsigned PollMethod::size() const
{
	return static_cast<unsigned>(m_fds.size());
}

SocketStatus PollMethod::select(int ms)
{
	auto result = poll(m_fds.data(), m_fds.size(), ms);
	if (result == 0)
		throw error::Timeout("select");
	if (result < 0)
#if defined(_WIN32)
		throw error::Error("poll", Socket::syserror(), WSAGetLastError());
#else
		throw error::Error("poll", Socket::syserror(), errno);
#endif

	for (auto &fd : m_fds)
		if (fd.revents != 0)
			return { m_lookup[fd.fd], todirection(fd.revents) };

	throw error::Error("select", "No socket found", 0);
}

std::vector<SocketStatus> PollMethod::selectMultiple(int ms)
{
	auto result = poll(m_fds.data(), m_fds.size(), ms);
	if (result == 0)
		throw error::Timeout("select");
	if (result < 0)
#if defined(_WIN32)
		throw error::Error("poll", Socket::syserror(), WSAGetLastError());
#else
		throw error::Error("poll", Socket::syserror(), errno);
#endif

	std::vector<SocketStatus> sockets;
	for (auto &fd : m_fds)
		if (fd.revents != 0)
			sockets.push_back({ m_lookup[fd.fd], todirection(fd.revents) });

	return sockets;
}

} // !namespace

#endif // !_SOCKET_LISTENER_HAVE_POLL

/* --------------------------------------------------------
 * Socket listener
 * -------------------------------------------------------- */

SocketListener::SocketListener(int method)
{
#if defined(SOCKET_LISTENER_HAVE_POLL)
	if (method == Poll)
		m_interface = std::make_unique<PollMethod>();
	else
#endif
		m_interface = std::make_unique<SelectMethod>();

	(void)method;
}

SocketListener::SocketListener(std::initializer_list<std::pair<Socket, int>> list, int method)
	: SocketListener(method)
{
	for (const auto &p : list)
		add(p.first, p.second);
}

} // !malikania

/*
 * NetworkManager.h -- processes clients
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

#ifndef _MALIKANIA_NETWORK_MANAGER_H_
#define _MALIKANIA_NETWORK_MANAGER_H_

#include <atomic>
#include <cassert>
#include <map>
#include <memory>
#include <thread>
#include <vector>

#include <malikania/ElapsedTimer.h>
#include <malikania/Hash.h>
#include <malikania/NetworkUtil.h>
#include <malikania/SocketListener.h>
#include <malikania/SocketSsl.h>
#include <malikania/SocketTcp.h>

#include "NetworkConnection.h"

namespace malikania {

class NetworkClient;
class ServerSettings;

/**
 * @class NetworkManager
 * @brief Receive clients, authenticate them and processes the data
 *
 * This class is the most high-level class of networking process.
 */
class NetworkManager {
private:
	/**
	 * @class UnidentifiedClientBase
	 * @brief Base class for unidentified clients
	 *
	 * This class just provide an elapsed timer and derives from the
	 * appropriate NetworkConnection type.
	 */
	template <typename Connection>
	class UnidentifiedClientBase : public Connection {
	protected:
		ElapsedTimer m_timer;

	public:
		/**
		 * Bring constructors.
		 */
		using Connection::Connection;

		/**
		 * Get the number of milliseconds elapsed.
		 */
		inline unsigned elapsed() noexcept
		{
			return m_timer.elapsed();
		}
	};

	/**
	 * @class UnidentifiedClientSsl
	 * @brief
	 */
	class UnidentifiedClientSsl : public UnidentifiedClientBase<NetworkConnectionSsl> {
	private:
		std::string m_hash;
		std::string m_result;
		unsigned m_id;

	public:
		inline UnidentifiedClientSsl(SocketSsl sc, std::string hash, unsigned id)
			: UnidentifiedClientBase<NetworkConnectionSsl>(std::move(sc))
			, m_hash(std::move(hash))
			, m_id(id)
		{
			m_result = Hash::sha256(m_hash + std::to_string(id));
		}

		inline const std::string &result() const noexcept
		{
			return m_result;
		}

		inline unsigned id() const noexcept
		{
			return m_id;
		}
	};

	/**
	 *
	 */
	using UnidentifiedClient = UnidentifiedClientBase<NetworkConnection>;

private:
	/* Master sockets */
	SocketListener m_listener;
	SocketTcp m_master;
	SocketSsl m_masterSsl;

	/* Thread */
	std::thread m_thread;
	std::atomic<bool> m_running{false};

	/* Identified clients */
	std::map<Socket, std::shared_ptr<NetworkClient>> m_lookup;
	std::map<Socket, std::shared_ptr<NetworkClient>> m_lookupSsl;

	/* Not identified clients */
	std::map<Socket, UnidentifiedClient> m_anon;
	std::map<Socket, UnidentifiedClientSsl> m_anonSsl;

	/* Accept routines */
	void acceptStandard(SocketTcp &sc);
	void acceptSsl(SocketSsl &sc);
	void accept(Socket &);

	/* Socket I/O routines */
	void flushUnidentifiedStandard(Socket &sc, int flags);
	void flushUnidentifiedSsl(Socket &sc, int flags);
	void flushStandard(Socket &sc, int flags);
	void flushSsl(Socket &sc, int flags);
	void flush(Socket &sc, int flags);

	/* Tests */
	bool isUnidentifiedStandard(const Socket &sc) const noexcept;
	bool isUnidentifiedSsl(const Socket &sc) const noexcept;
	bool isStandard(const Socket &sc) const noexcept;
	bool isSsl(const Socket &sc) const noexcept;
	bool isMaster(const Socket &sc) const noexcept;

	/* Clean and close */
	template <typename Map>
	void cleanUnidentified(Map &map)
	{
		/*
		 * Remove people that have not identified for 5 seconds
		 */
		for (auto it = map.begin(); it != map.end(); ) {
			if (it->second.elapsed() > 5000) {
				printf("network: -> removing due to inactivity\n");

				m_listener.remove(it->second.socket());

				it->second.socket().close();
				it = map.erase(it);
			} else {
				it ++;
			}
		}
	}

	template <typename UnidentifiedMap>
	void closeUnidentified(UnidentifiedMap &map)
	{
		for (auto &pair : map) {
			pair.second.socket().close();
		}
	}

	void run();

public:
	/**
	 * Create the network service. This does not create the thread.
	 */
	NetworkManager(const ServerSettings &);

	/**
	 * Close the network service.
	 *
	 * @throw std::runtime_error if stop() has not been called
	 */
	~NetworkManager();

	/**
	 * Tells if the network service is started.
	 *
	 * @return true if started
	 */
	inline bool isRunning() const noexcept
	{
		return m_running;
	}

	/**
	 * Start the thread.
	 *
	 * @throw std::runtime_error if the thread is started
	 */
	void start();

	/**
	 * Stop the thread.
	 *
	 * @throw std::runtime_error if the thread is already stopped
	 */
	void stop();
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_MANAGER_H_
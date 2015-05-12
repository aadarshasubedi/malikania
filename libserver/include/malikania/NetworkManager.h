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

namespace malikania {

class NetworkClient;
class ServerSettings;

class NetworkManager {
private:
	template <typename Sock>
	class AnonymousBase {
	protected:
		Sock m_socket;
		ElapsedTimer m_timer;
		std::string m_input;
		std::string m_output;

	public:
		inline AnonymousBase(Sock sc)
			: m_socket(std::move(sc))
		{
		}

		inline Sock &socket() noexcept
		{
			return m_socket;
		}

		inline void append(std::string output)
		{
			m_output += output;
		}

		inline bool hasOutput() const noexcept
		{
			return !m_output.empty();
		}

		inline void send()
		{
			assert(hasOutput());

			unsigned nsent = m_socket.send(m_output);

			m_output.erase(0, nsent);
		}

		inline void read()
		{
			m_input += m_socket.recv(512);
		}

		inline std::vector<std::string> data()
		{
			return NetworkUtil::split(m_input);
		}
	};

	using Anonymous = AnonymousBase<SocketTcp>;

	class AnonymousSsl : public AnonymousBase<SocketSsl> {
	private:
		std::string m_hash;
		std::string m_result;
		unsigned m_id;

	public:
		inline AnonymousSsl(SocketSsl sc, std::string hash, unsigned id)
			: AnonymousBase<SocketSsl>(std::move(sc))
			, m_hash(std::move(hash))
			, m_id(id)
		{
			m_result = hash + std::to_string(id);
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

private:
	/* Master sockets */
	SocketListener m_listener;
	SocketTcp m_master;
	SocketSsl m_masterSsl;

	/* Thread */
	std::thread m_thread;
	std::atomic<bool> m_running{true};

	/* Identified clients */
	std::map<Socket, std::shared_ptr<NetworkClient>> m_lookup;
	std::map<Socket, std::shared_ptr<NetworkClient>> m_lookupSsl;

	/* Not identified clients */
	std::map<Socket, Anonymous> m_anon;
	std::map<Socket, AnonymousSsl> m_anonSsl;

	/* Accept routines */
	void acceptStandard(SocketTcp &sc);
	void acceptSsl(SocketSsl &sc);
	void accept(Socket &);

	/* Socket I/O routines */
	void flushAnonymousStandard(Socket &sc, int flags);
	void flushAnonymousSsl(Socket &sc, int flags);
	void flushStandard(Socket &sc, int flags);
	void flushSsl(Socket &sc, int flags);
	void flush(Socket &sc, int flags);

	/* Tests */
	bool isAnonymousStandard(const Socket &sc) const noexcept;
	bool isAnonymousSsl(const Socket &sc) const noexcept;
	bool isStandard(const Socket &sc) const noexcept;
	bool isSsl(const Socket &sc) const noexcept;
	bool isMaster(const Socket &sc) const noexcept;

	void run();

public:
	NetworkManager(const ServerSettings &);
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_MANAGER_H_

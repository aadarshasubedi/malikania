#ifndef _MALIKANIA_NETWORK_MANAGER_H_
#define _MALIKANIA_NETWORK_MANAGER_H_

#include <atomic>
#include <map>
#include <memory>
#include <thread>
#include <vector>

#include <malikania/ElapsedTimer.h>
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

		inline void send(std::string output)
		{
			m_output += output;
		}
	};

	using Anonymous = AnonymousBase<SocketTcp>;

	class AnonymousSsl : public AnonymousBase<SocketSsl> {
	private:
		std::string m_hash;
		unsigned m_id;

	public:
		inline AnonymousSsl(SocketSsl sc, std::string hash, unsigned id)
			: AnonymousBase<SocketSsl>(std::move(sc))
			, m_hash(std::move(hash))
			, m_id(id)
		{
		}

		inline unsigned id() const noexcept
		{
			return m_id;
		}
	};

private:
	/* Master sockets */
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

	void processAcceptStandard(SocketTcp &sc);
	void processAcceptSsl(SocketSsl &sc);
	void processAccept(Socket &);

	void processAnonymous(Socket &c, int flags);
	void processClient(Socket &sc, int flags);
	void process(Socket &sc, int flags);

	bool isMaster(const Socket &sc) const noexcept;
	bool isAnonymous(const Socket &sc) const noexcept;

	void run();

public:
	NetworkManager(const ServerSettings &);
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_MANAGER_H_

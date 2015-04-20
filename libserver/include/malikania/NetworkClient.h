#ifndef _MALIKANIA_NETWORK_CLIENT_H_
#define _MALIKANIA_NETWORK_CLIENT_H_

#include <functional>
#include <mutex>
#include <string>

#include <malikania/SocketTcp.h>
#include <malikania/SocketSsl.h>

namespace malikania {

class NetworkClient {
private:
	std::string m_input;
	std::string m_output;
	std::string m_inputSsl;
	std::string m_outputSsl;

	std::function<void ()> m_onDisconnect;
	std::function<void (std::string)> m_onMessage;
	std::function<void (std::string)> m_onSslMessage;

public:
	inline void setOnDisconnect(std::function<void ()> func) noexcept
	{
		m_onDisconnect = std::move(func);
	}

	inline void setOnMessage(std::function<void (std::string)> func) noexcept
	{
		m_onMessage = std::move(func);
	}

	inline void setOnSslMessage(std::function<void (std::string)> func) noexcept
	{
		m_onSslMessage = std::move(func);
	}
};

} // !malikania

#endif // !_MALIKANIA_NETWORK_CLIENT_H_


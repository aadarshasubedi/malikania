#ifndef _MALIKANIA_SERVER_APPLICATION_H_
#define _MALIKANIA_SERVER_APPLICATION_H_

#include <memory>
#include <type_traits>

#include <malikania/Application.h>

#include "ServerLoaderInterface.h"

namespace malikania {

class ServerApplication : public Application {
private:
	std::unique_ptr<ServerLoaderInterface> m_loader;

public:
	using Application::Application;

	void run();

	template <typename Loader>
	void run(Loader &&loader)
	{
		m_loader = std::make_unique<std::decay_t<Loader>>(std::forward<Loader>(loader));

		run();
	}
};

} // !malikania

#endif // !_MALIKANIA_SERVER_APPLICATION_H_


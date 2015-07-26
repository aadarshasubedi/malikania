#include <iostream>

#include "Database.h"

namespace malikania {

std::mutex Database::s_mutex;
std::atomic<bool> Database::s_initialized{false};

std::string Database::makeUri(const DatabaseInfo &info)
{
	std::string uri;
	return uri;
}

void Database::initialize()
{
	mongoc_init();
}

void Database::cleanup()
{
	mongoc_cleanup();
}

Database::Database(const DatabaseInfo &info)
{
	if(!s_initialized) {
		std::lock_guard<std::mutex> lock(s_mutex);

		s_initialized = true;
		initialize();
		atexit(&Database::cleanup);
	}

	m_handle = ClientHandle(mongoc_client_new("mongodb://localhost:27017/"), mongoc_client_destroy);

	if(m_handle != nullptr) {

	}
}

} // !malikania

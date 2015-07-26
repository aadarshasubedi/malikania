#ifndef DATABASE_H
#define DATABASE_H

#include <atomic>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <string>

#include <bson.h>
#include <mongoc.h>

namespace malikania {

class DatabaseInfo {
public:
	std::string host;
	int port{27017};
	std::string database;
	std::string collection;
};

class Database {
private:
	using ClientHandle = std::unique_ptr<mongoc_client_t, void (*)(mongoc_client_t *)>;

	static std::mutex s_mutex;
	static std::atomic<bool> s_initialized;

	ClientHandle m_handle{nullptr, nullptr};

	std::string makeUri(const DatabaseInfo &info);
	void initialize();
	static void cleanup();

public:
	Database(const DatabaseInfo &info);
};

}// !malikania

#endif // DATABASE_H


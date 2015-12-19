/*
 * DataBase.h -- class to manage database in mongodb
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

#ifndef _MALIKIANIA_DATABASE_H_
#define _MALIKIANIA_DATABASE_H_

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
	using DatabaseHandle = std::unique_ptr<mongoc_database_t, void (*)(mongoc_database_t *)>;

	static std::mutex s_mutex;
	static std::atomic<bool> s_initialized;

	ClientHandle m_handle{nullptr, nullptr};
	DatabaseHandle m_database{nullptr, nullptr};
	DatabaseInfo m_info;

	std::string makeUri(const DatabaseInfo &info);
	void initialize();

public:
	Database(DatabaseInfo info);
	static void cleanup();

	void insert();
};

}// !malikania

#endif // !_MALIKIANIA_DATABASE_H_


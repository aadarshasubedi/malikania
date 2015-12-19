/*
 * Database.cpp -- class to manage database in mongodb
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

#include <iostream>
#include <sstream>

#include "Database.h"

namespace malikania {

std::mutex Database::s_mutex;
std::atomic<bool> Database::s_initialized{false};

std::string Database::makeUri(const DatabaseInfo &info)
{
	std::string uri;
	std::stringstream ss;
	ss << "mongodb://" << info.host << ":" <<info.port << "/";
	uri = ss.str();
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

Database::Database(DatabaseInfo info)
	: m_info(std::move(info))

{
	if (!s_initialized) {
		std::lock_guard<std::mutex> lock(s_mutex);
		s_initialized = true;
		initialize();
		atexit(&Database::cleanup);
	}

	m_handle = ClientHandle(mongoc_client_new(makeUri(m_info).c_str()), mongoc_client_destroy);
	m_database = DatabaseHandle(mongoc_client_get_database(m_handle.get(), m_info.database.c_str()), mongoc_database_destroy);

	if (m_handle != nullptr) {

	}
}

void Database::insert()
{
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;

	collection = mongoc_client_get_collection(m_handle.get(), m_info.database.c_str(), m_info.collection.c_str());
	doc = bson_new ();
	BSON_APPEND_UTF8 (doc, "name", "Nanahara");
	BSON_APPEND_INT32 (doc, "lvl", 50);

	if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
		fprintf (stderr, "%s\n", error.message);
	}

	//free mongodb object
	mongoc_collection_destroy (collection);

}

} // !malikania

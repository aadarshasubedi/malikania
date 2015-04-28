/*
 * main.cpp -- main client files
 *
 * Copyright (c) 2014 David Demelier <markand@malikania.fr>
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

#include <malikania/Json.h>
#include <malikania/Hash.h>
#include <malikania/SocketAddress.h>
#include <malikania/SocketSsl.h>
#include <malikania/SocketTcp.h>
#include <malikania/SocketListener.h>

using namespace malikania;

class IdentifyParams {
public:
	int id;
	std::string hash;
};

IdentifyParams step1(SocketSsl &sc)
{
	sc.connect(address::Internet("localhost", 24010, AF_INET));

	SocketListener listener{
		{ sc, SocketListener::Read }
	};

	std::string input;
	while (input.find("\r\n\r\n") == std::string::npos) {
		listener.wait(30000);

		input += sc.recv(512);
	}

	JsonObject object = JsonDocument(input).toObject();

	if (!object.contains("command") || object["command"].toString() != "identify-req") {
		throw std::invalid_argument("invalid message received");
	}
	if (!object.contains("id")) {
		throw std::invalid_argument("missing id");
	}
	if (!object.contains("hash")) {
		throw std::invalid_argument("missing hash");
	}

	return IdentifyParams{
		object["id"].toInteger(),
		object["hash"].toString()
	};
}

void step2(SocketTcp &sc, const std::string &str)
{
	sc.connect(address::Internet("localhost", 24000, AF_INET));
	sc.send(str);
}

int main(void)
{
	try {
		SocketSsl socketSsl(AF_INET, 0);
		SocketTcp socketTcp(AF_INET, 0);

		puts("Step 1 begins");
		auto params = step1(socketSsl);
		puts("Step 1 ends");
		printf("  - id: %d\n", params.id);
		printf("  - hash: %s\n", params.hash.c_str());

		std::ostringstream oss;
		puts("Step 2 begins");
		oss << "{"
		    << "\"command\":\"identify-req\","
		    << "\"result\":\"" << Hash::sha256(params.hash + std::to_string(params.id)) << "\""
		    << "}\r\n\r\n";
		step2(socketTcp, oss.str());
		puts("Step 2 ends");
		puts("Successfully connected!");
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}

	return 0;
}

/*
 * Hash.h -- hash functions
 *
 * Copyright (c) 2013, 2014 David Demelier <markand@malikania.fr>
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

#ifndef _HASH_H_
#define _HASH_H_

/**
 * @file Hash.h
 * @brief Hash functions
 */

#include <string>

#include "Common.h"

namespace malikania {

/**
 * @class Hash
 * @brief Hash functions
 *
 * Provide support for MD5, SHA1, SHA256 and SHA512.
 */
class MALIKANIA_COMMON_EXPORT Hash {
private:
	template <typename Context>
	using Init	= int (*)(Context *);

	template <typename Context>
	using Update	= int (*)(Context *, const void *, size_t);

	template <typename Context>
	using Final	= int (*)(unsigned char *, Context *);

	template <typename Context, size_t Length>
	static std::string convert(const std::string &input,
				   Init<Context> init,
				   Update<Context> update,
				   Final<Context> finalize)
	{
		unsigned char digest[Length];
		char hash[Length * 2 + 1];
		
		Context ctx;
		init(&ctx);
		update(&ctx, input.c_str(), input.length());
		finalize(digest, &ctx);
		
		for (unsigned long i = 0; i < Length; i++)
			sprintf(&hash[i * 2], "%02x", (unsigned int)digest[i]);
		
		return std::string(hash);
	}

public:
	/**
	 * Hash using MD5.
	 *
	 * @param input the input string
	 * @return the hashed string
	 */
	static std::string md5(const std::string &input);

	/**
	 * Hash using SHA1.
	 *
	 * @param input the input string
	 * @return the hashed string
	 */
	static std::string sha1(const std::string &input);

	/**
	 * Hash using SHA256.
	 *
	 * @param input the input string
	 * @return the hashed string
	 */
	static std::string sha256(const std::string &input);

	/**
	 * Hash using SHA512.
	 *
	 * @param input the input string
	 * @return the hashed string
	 */
	static std::string sha512(const std::string &input);
};

} // !malikania

#endif // !_HASH_H_

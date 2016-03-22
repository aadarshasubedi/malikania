/*
 * Hash.cpp -- hash functions
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

#include "Hash.h"

#include <openssl/sha.h>
#include <openssl/md5.h>

namespace malikania {

std::string Hash::md5(const std::string &input)
{
	return convert<MD5_CTX, MD5_DIGEST_LENGTH>(input, MD5_Init, MD5_Update, MD5_Final);
}

std::string Hash::sha1(const std::string &input)
{
	return convert<SHA_CTX, SHA_DIGEST_LENGTH>(input, SHA1_Init, SHA1_Update, SHA1_Final);
}

std::string Hash::sha256(const std::string &input)
{
	return convert<SHA256_CTX, SHA256_DIGEST_LENGTH>(input, SHA256_Init, SHA256_Update, SHA256_Final);
}

std::string Hash::sha512(const std::string &input)
{
	return convert<SHA512_CTX, SHA512_DIGEST_LENGTH>(input, SHA512_Init, SHA512_Update, SHA512_Final);
}

} // !malikania

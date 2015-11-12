/*
 * Util.h -- utilities
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

#ifndef _UTIL_H_
#define _UTIL_H_

/**
 * @file NetworkUtil.h
 * @brief Some utilities for network
 */

#include <string>
#include <vector>

namespace malikania {

/**
 * Various utilities.
 */
namespace util {

/**
 * Split the network message buffer by \r\n\r\n and update it in-place.
 *
 * @param input the buffer to split and update
 * @return the list of received message or empty if not ready
 */
std::vector<std::string> netsplit(std::string &input);

} // !util

} // !malikania

#endif // !_NETWORK_UTIL_H_

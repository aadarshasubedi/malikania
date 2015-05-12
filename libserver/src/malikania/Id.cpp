/*
 * Id.cpp -- player id generator
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

#include "Id.h"

namespace malikania {

unsigned Id::m_current{0};
std::priority_queue<unsigned> Id::m_reusable;

unsigned Id::next() noexcept
{
	unsigned id;

	if (m_reusable.size() > 0) {
		id = m_reusable.top();
		m_reusable.pop();
	} else {
		id = m_current++;
	}

	return id;
}

void Id::release(unsigned id) noexcept
{
	m_reusable.push(id);
}

void Id::reset() noexcept
{
	m_current = 0;

	while (!m_reusable.empty()) {
		m_reusable.pop();
	}
}

} // !malikania

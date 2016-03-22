/*
 * Id.h -- integer id generator
 *
 * Copyright (c) 2013-2016 Malikania Authors
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

#ifndef _MALIKANIA_COMMON_ID_H_
#define _MALIKANIA_COMMON_ID_H_

/**
 * @file Id.h
 * @brief Integer id generator
 */

#include <limits>
#include <queue>
#include <stdexcept>

#include "Common.h"

namespace malikania {

/**
 * @class IdGen
 * @brief Integer id generator
 *
 * This class helps generating and release unique integer id that can be used anywhere. The ids are generated in a
 * sequence and when an id is released it is reused instead of incrementing the next number.
 *
 * The template can use any integral integer but unsigned are preferred.
 *
 * The maximum number of id is equal to std::numeric_limits<T>::max - 1.
 */
template <typename T>
class MALIKANIA_COMMON_EXPORT IdGen {
private:
	static_assert(std::numeric_limits<T>::is_integer, "IdGen requires integral types");

	T m_current{0};
	std::priority_queue<T> m_reusable;

public:
	/**
	 * Get the next id for that player.
	 *
	 * @return the id
	 * @throw std::out_of_range if no number is available
	 */
	T next();

	/**
	 * Release the player id.
	 *
	 * @param id the id not needed anymore
	 */
	inline void release(T id) noexcept
	{
		m_reusable.push(id);
	}

	/**
	 * Reset the ids to 0 and remove the queue.
	 */
	void reset() noexcept;
};

template <typename T>
T IdGen<T>::next()
{
	T id;

	if (m_reusable.size() > 0) {
		id = m_reusable.top();
		m_reusable.pop();
	} else {
		if (m_current == std::numeric_limits<T>::max()) {
			throw std::out_of_range("no id available");
		}

		id = m_current++;
	}

	return id;
}

template <typename T>
void IdGen<T>::reset() noexcept
{
	m_current = 0;

	while (!m_reusable.empty()) {
		m_reusable.pop();
	}
}

/**
 * @class Id
 * @brief RAII based id owner
 *
 * This class is similar to a std::lock_guard or std::unique_lock in a way that the id is acquired
 * when the object is instanciated and released when destroyed.
 *
 * This class does not take ownership of the IdGen so it must still exists when the Id is destroyed.
 */
template <typename T>
class Id {
private:
	IdGen<T> &m_gen;
	T m_id;

public:
	/**
	 * Construct a new Id and take the next number.
	 *
	 * @param gen the generator
	 * @throw any exception if IdGen fails to give an id.
	 */
	inline Id(IdGen<T> &gen)
		: m_gen(gen)
		, m_id(m_gen.next())
	{
	}

	/**
	 * Construct an Id with an already taken number.
	 *
	 * @param gen the generator
	 * @param id the id
	 * @warning be sure that the id was taken from this generator
	 */
	Id(IdGen<T> &gen, T id)
		: m_gen(gen)
		, m_id(id)
	{
	}

	/**
	 * Destroy the id and release the number.
	 */
	~Id() noexcept
	{
		m_gen.release(m_id);
	}

	/**
	 * Get the number id.
	 *
	 * @return the id
	 */
	inline T value() const noexcept
	{
		return m_id;
	}

	/**
	 * Convert the id to the number.
	 */
	inline operator T() const noexcept
	{
		return m_id;
	}
};

} // !malikania

#endif // !_MALIKANIA_ID_H_

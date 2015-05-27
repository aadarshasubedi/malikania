/*
 * Js.h -- JavaScript support in Malikania Engine
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

#ifndef _MALIKANIA_JS_H_
#define _MALIKANIA_JS_H_

/**
 * @file Js.h
 * @brief JavaScript support in Malikania Engine and Duktape wrappers
 */

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <duktape.h>

namespace malikania {

/**
 * Handle for Duktape object.
 */
using JsHandle = std::unique_ptr<duk_context, void (*)(duk_context *)>;

/**
 * @class Js
 * @brief Wrap Duktape
 */
class Js : public JsHandle {
public:
	/**
	 * Construct a Duktape heap.
	 */
	Js();

	/**
	 * Convert the instance to the Duktape pointer.
	 *
	 * @return the pointer
	 */
	inline operator duk_context *() const noexcept
	{
		return get();
	}
};

/**
 * @class JsRef
 * @brief Store a reference and release it when needed.
 *
 * This class wraps the dukx_unref automatically so you don't need to remember
 * when to release references.
 *
 * It is assignable by move semantics but is not copiable because the destructor
 * automatically remove the reference.
 */
class JsRef {
private:
	duk_context *m_context{nullptr};
	int m_reference{-1};

	JsRef(const JsRef &) = delete;
	JsRef &operator=(const JsRef &) = delete;

public:
	/**
	 * Default constructor. No reference.
	 */
	JsRef() = default;

	/**
	 * Construct an assigned reference.
	 *
	 * @param ctx
	 * @param reference
	 */
	inline JsRef(duk_context *ctx, int reference) noexcept
		: m_context(ctx)
		, m_reference(reference)
	{
	}

	/**
	 * Move constructor. The other value is being null.
	 *
	 * @param other the other value
	 */
	JsRef(JsRef &&other) noexcept;

	/**
	 * Move assigment. The other value is being null.
	 *
	 * @param other the other value
	 * @return *this
	 */
	JsRef &operator=(JsRef &&other) noexcept;

	/**
	 * Destroy the value and unref it.
	 */
	~JsRef();

	/**
	 * Check if the reference is valid.
	 *
	 * @return true if valid
	 */
	inline operator bool() const noexcept
	{
		return m_context != nullptr && m_reference != -1;
	}
	/**
	 * Get the reference as an implicit cast.
	 *
	 * @throw std::invalid_argument if the reference is null
	 */
	operator int() const
	{
		if (m_reference < 0) {
			throw std::invalid_argument("attempt to get null JavaScript reference");
		}

		return m_reference;
	}
};

#if !defined(NDEBUG)
#define dukx_assert_begin(ctx)						\
	int _topstack = duk_get_top(ctx)
#else
#define dukx_assert_begin(ctx)
#endif

#if !defined(NDEBUG)
#define dukx_assert_equals(ctx)						\
	assert(_topstack == duk_get_top(ctx))
#else
#define dukx_assert_equals(ctx)
#endif

#if !defined(NDEBUG)
#define dukx_assert_end(ctx, count)					\
	assert(_topstack == (duk_get_top(ctx) - count))
#else
#define dukx_assert_end(ctx, count)
#endif

/**
 * Push a boolean value.
 *
 * @param ctx the Duktape context
 * @param value the value
 * @return 1
 */
inline duk_ret_t dukx_push(duk_context *ctx, bool value)
{
	duk_push_boolean(ctx, value);

	return 1;
}

/**
 * Push an integer value.
 *
 * @param ctx the Duktape context
 * @param value the value
 * @return 1
 */
inline duk_ret_t dukx_push(duk_context *ctx, int value)
{
	duk_push_int(ctx, value);

	return 1;
}

/**
 * Push a C string.
 *
 * @warning This function does not embed \0.
 * @param ctx the Duktape context
 * @param value the value
 * @return 1
 */
inline duk_ret_t dukx_push(duk_context *ctx, const char *value)
{
	duk_push_string(ctx, value);

	return 1;
}

/**
 * Push a C++ string.
 *
 * @note This function embeds \0
 * @param ctx the Duktape context
 * @param value the value
 * @return 1
 */
inline duk_ret_t dukx_push(duk_context *ctx, const std::string &value)
{
	duk_push_lstring(ctx, value.c_str(), value.length());

	return 1;
}

/**
 * Push a real number.
 *
 * @param ctx the Duktape context
 * @param value the value
 * @return 1
 */
inline duk_ret_t dukx_push(duk_context *ctx, double value)
{
	duk_push_number(ctx, value);

	return 1;
}

/**
 * Push the null value.
 *
 * @param ctx the Duktape context
 * @param value the value
 * @return 1
 */
inline duk_ret_t dukx_push(duk_context *ctx, std::nullptr_t)
{
	duk_push_null(ctx);

	return 1;
}

/**
 * Push undefined value.
 *
 * @param ctx the Duktape context
 * @param value the value
 * @return 1
 */
inline duk_ret_t dukx_push(duk_context *ctx) noexcept
{
	duk_push_undefined(ctx);

	return 1;
}

/**
 * Get a boolean value.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_get(duk_context *ctx, duk_idx_t index, bool &value)
{
	value = duk_get_boolean(ctx, index);
}

/**
 * Get an integer value.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_get(duk_context *ctx, duk_idx_t index, int &value)
{
	value = duk_get_int(ctx, index);
}

/**
 * Get a string value.
 *
 * @note The string may have embedded \0
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_get(duk_context *ctx, duk_idx_t index, std::string &value)
{
	size_t size;
	const char *str = duk_get_lstring(ctx, index, &size);

	value = {str, size};
}

/**
 * Get a real value.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_get(duk_context *ctx, duk_idx_t index, double &value)
{
	value = duk_get_number(ctx, index);
}

/**
 * Requires a boolean value.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_require(duk_context *ctx, duk_idx_t index, bool &value)
{
	value = duk_require_boolean(ctx, index);
}

/**
 * Requires an integer value.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_require(duk_context *ctx, duk_idx_t index, int &value)
{
	value = duk_require_int(ctx, index);
}

/**
 * Requires a string value.
 *
 * @note The string may have embedded \0
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_require(duk_context *ctx, duk_idx_t index, std::string &value)
{
	size_t size;
	const char *str = duk_require_lstring(ctx, index, &size);

	value = {str, size};
}

/**
 * Requires a real value.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @param value the value reference
 */
inline void dukx_require(duk_context *ctx, duk_idx_t index, double &value)
{
	value = duk_require_number(ctx, index);
}

/**
 * Convenient template dukx_get wrapper.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @return the value
 */
template <typename T>
inline T dukx_get(duk_context *ctx, duk_idx_t index)
{
	T value{};

	dukx_get(ctx, index, value);

	return value;
}

/**
 * Convenient template dukx_require wrapper.
 *
 * @param ctx the Duktape context
 * @param index the value index
 * @return the value
 */
template <typename T>
inline T dukx_require(duk_context *ctx, duk_idx_t index)
{
	T value{};

	dukx_require(ctx, index, value);

	return value;
}

/**
 * Reference the object at the top of stack into the registry.
 *
 * @param ctx the context
 * @return the reference
 * @see dukx_unref
 * @see dukx_refget
 */
int dukx_ref(duk_context *ctx);

/**
 * Get a referenced object. May throw a JavaScript exception.
 *
 * @param ctx the context
 * @param ref the reference
 */
void dukx_refget(duk_context *ctx, int ref);

/**
 *
 * @param ctx
 * @param arrayIndex
 * @param ref
 */
void dukx_unref(duk_context *ctx, int ref);

} // !malikania

#endif // !_MALIKANIA_JS_H_

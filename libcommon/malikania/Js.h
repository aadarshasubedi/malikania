/*
 * Js.h -- JavaScript support in Malikania Engine
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

#ifndef _MALIKANIA_JS_H_
#define _MALIKANIA_JS_H_

/**
 * @file Js.h
 * @brief JavaScript support in Malikania Engine and Duktape wrappers
 */

#include <memory>
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

/*
 * Private helpers.
 */
namespace {

inline duk_ret_t dukx_push_wrap(duk_context *, duk_ret_t count) noexcept
{
	return count;
}

template <typename T, typename... Args>
inline duk_ret_t dukx_push_wrap(duk_context *ctx, duk_ret_t count, T &&value, Args&&... args)
{
	dukx_push(ctx, std::forward<T>(value));

	return dukx_push_wrap(ctx, count + 1, std::forward<Args>(args)...);
}

} // !namespace

/**
 * Convenient variadic dukx_push function. This function takes at least
 * two values to avoid be called when calling dukx_push with only
 * one argument (plus the context).
 *
 * @param ctx the Duktape context
 * @param value1 the first value
 * @param value2 the second value
 * @param args the next arguments
 * @return the number of arguments pushed
 */
template <typename T1, typename T2, typename... Args>
inline duk_ret_t dukx_push(duk_context *ctx, T1 &&value1, T2 &&value2, Args&&... args)
{
	return dukx_push_wrap(ctx, 0, std::forward<T1>(value1), std::forward<T2>(value2), std::forward<Args>(args)...);
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

} // !malikania

#endif // !_MALIKANIA_JS_H_

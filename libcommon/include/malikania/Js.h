/*
 * Js.h -- JavaScript wrapper for Duktape
 *
 * Copyright (c) 2013, 2014, 2015 David Demelier <markand@malikania.fr>
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

#ifndef _JS_H_
#define _JS_H_

/**
 * @file Js.h
 * @brief Bring JavaScript using Duktape
 *
 * This file provides usual Duktape function renamed and placed into `js` namespace. It also replaces error
 * code with exceptions when possible.
 *
 * For convenience, this file also provides templated functions, overloads and much more.
 */

#include <cassert>
#include <cerrno>
#include <cstring>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include <duktape.h>

namespace malikania {

namespace js {

#if !defined(NDEBUG)
#define assertBegin(ctx)						\
	int _topstack = duk_get_top(ctx)
#else
#define assertBegin(ctx)
#endif

#if !defined(NDEBUG)
#define assertEquals(ctx)						\
	assert(_topstack == duk_get_top(ctx))
#else
#define assertEquals(ctx)
#endif

#if !defined(NDEBUG)
#define assertEnd(ctx, count)					\
	assert(_topstack == (duk_get_top(ctx) - count))
#else
#define assertEnd(ctx, count)
#endif

/**
 * Typedef for readability.
 */
using ContextPtr = duk_context *;

/**
 * @class Object
 * @brief Empty class tag for push() function
 */
class Object {
};

/**
 * @class Arary
 * @brief Empty class tag for push() function
 */
class Array {
};

/**
 * @class Function
 * @brief Duktape/C function definition
 */
class Function {
public:
	/**
	 * The function pointer, must not be null.
	 */
	duk_c_function function{nullptr};

	/**
	 * Number of args that the function takes
	 */
	duk_idx_t nargs{0};
};

/**
 * @class ErrorInfo
 * @brief Error description
 *
 * This class fills the fields got in an Error object.
 */
class ErrorInfo : public std::exception {
public:
	std::string name;		//!< name of error
	std::string message;		//!< error message
	std::string stack;		//!< stack if available
	std::string fileName;		//!< filename if applicable
	int lineNumber{0};		//!< line number if applicable

	/**
	 * Get the error message. This effectively returns message field.
	 *
	 * @return the message
	 */
	const char *what() const noexcept override
	{
		return message.c_str();
	}
};

template <typename Type>
class TypeInfo {
public:
};

/**
 * @class Context
 * @brief RAII based Duktape handler
 *
 * This class is implicitly convertible to duk_context for convenience.
 */
class Context {
private:
	using Deleter = void (*)(duk_context *);
	using Handle = std::unique_ptr<duk_context, Deleter>;

	Handle m_handle;

	/* Move and copy forbidden */
	Context(const Context &) = delete;
	Context &operator=(const Context &) = delete;
	Context(const Context &&) = delete;
	Context &operator=(const Context &&) = delete;


public:
	inline Context()
		: m_handle{duk_create_heap_default(), duk_destroy_heap}
	{
	}

	inline Context(ContextPtr ctx) noexcept
		: m_handle{ctx, [] (ContextPtr) {}}
	{
	}

	/**
	 * Convert the context to the native Duktape/C type.
	 *
	 * @return the duk_context
	 */
	inline operator duk_context *() noexcept
	{
		return m_handle.get();
	}

	/**
	 * Convert the context to the native Duktape/C type.
	 *
	 * @return the duk_context
	 */
	inline operator duk_context *() const noexcept
	{
		return m_handle.get();
	}

	/* ----------------------------------------------------------
	 * Push / Get / Require
	 * ---------------------------------------------------------- */

	template <typename Type>
	inline void push(Type &&value)
	{
		TypeInfo<std::decay_t<Type>>::push(*this, std::forward<Type>(value));
	}

	/**
	 * Generic template function to get a value from the stack.
	 *
	 * @param index the index
	 * @return the value
	 */
	template <typename Type>
	inline Type get(duk_idx_t index)
	{
		return TypeInfo<Type>::get(*this, index);
	}

	template <typename Type>
	inline Type require(duk_idx_t index)
	{
		return TypeInfo<Type>::require(*this, index);
	}

	/* --------------------------------------------------------
	 * Get functions (for object)
	 * -------------------------------------------------------- */

	template <typename Type>
	inline Type getObject(duk_idx_t index, const std::string &name)
	{
		assertBegin(m_handle.get());
		duk_get_prop_string(m_handle.get(), index, name.c_str());
		Type value = get<Type>(-1);
		duk_pop(m_handle.get());
		assertEquals(m_handle.get());

		return value;
	}

	/* --------------------------------------------------------
	 * Set functions (for object)
	 * -------------------------------------------------------- */

	template <typename Type>
	void setObject(duk_idx_t index, const std::string &name, Type&& value)
	{
		index = duk_normalize_index(m_handle.get(), index);

		assertBegin(m_handle.get());
		push(std::forward<Type>(value));
		duk_put_prop_string(m_handle.get(), index, name.c_str());
		assertEquals(m_handle.get());
	}

	/* --------------------------------------------------------
	 * Basic functions
	 * -------------------------------------------------------- */

	/**
	 * Get the type of the value at the specified index.
	 *
	 * @param ctx the context
	 * @param index the idnex
	 * @return the type
	 */
	duk_int_t type(duk_idx_t index);

	/**
	 * Get the current stack size.
	 *
	 * @param ctx the context
	 * @return the stack size
	 */
	int top();

	/**
	 * Pop a certain number of values from the top of the stack.
	 *
	 * @param ctx the context
	 * @param count the number of values to pop
	 * @pre count must be positive
	 */
	void pop(int count = 1);

	/**
	 * Check if idx1 is an instance of idx2.
	 *
	 * @param ctx the context
	 * @param idx1 the value to test
	 * @param idx2 the instance requested
	 * @return true if idx1 is instance of idx2
	 */
	duk_bool_t instanceof(duk_idx_t idx1, duk_idx_t idx2);

	/**
	 * Call the object at the top of the stack.
	 *
	 * @param ctx the context
	 * @param nargs the number of arguments
	 * @note Non-protected
	 */
	void call(duk_idx_t nargs = 0);

	/**
	 * Call in protected mode the object at the top of the stack.
	 *
	 * @param ctx the context
	 * @param nargs the number of arguments
	 * @throw ErrorInfo on errors
	 */
	void pcall(duk_idx_t nargs = 0);

	/* ------------------------------------------------------------------
	 * Eval functions
	 * ------------------------------------------------------------------ */

	/**
	 * Evaluate a non-protected chunk at the top of the stack.
	 *
	 * @param ctx
	 */
	void eval();

	/**
	 * Evaluate a non-protected string script.
	 *
	 * @param ctx the context
	 * @param script the script content
	 */
	void evalString(const std::string &script);

	/**
	 * Evaluate a non-protected file.
	 *
	 * @param ctx the context
	 * @param file the file
	 */
	void evalFile(const std::string &file);

	/**
	 * Evaluate a protected chunk.
	 *
	 * @param ctx the context
	 * @throw ErrorInfo the error
	 */
	void peval();

	/**
	 * Evaluate a protected string script.
	 *
	 * @param ctx the context
	 * @param script the script content
	 * @throw ErrorInfo the error
	 */
	void pevalString(const std::string &script);

	/**
	 * Evaluate a protected file.
	 *
	 * @param ctx the context
	 * @param file the file
	 * @throw ErrorInfo the error
	 */
	void pevalFile(const std::string &file);

	/* ------------------------------------------------------------------
	 * Extended functions
	 * ------------------------------------------------------------------ */

	/**
	 * Enumerate an object or an array at the specified index.
	 *
	 * @param ctx the context
	 * @param index the object or array index
	 * @param flags the optional flags to pass to duk_enum
	 * @param getvalue set to true if you want to extract the value
	 * @param func the function to call for each properties
	 */
	template <typename Func>
	void enumerate(duk_idx_t index, duk_uint_t flags, duk_bool_t getvalue, Func&& func)
	{
		assertBegin(m_handle.get());
		duk_enum(m_handle.get(), index, flags);

		while (duk_next(m_handle.get(), -1, getvalue)) {
			func(*this);
			duk_pop_n(m_handle.get(), 1 + (getvalue ? 1 : 0));
		}

		duk_pop(m_handle.get());
		assertEquals(m_handle.get());
	}

	/* --------------------------------------------------------
	 * Global functions
	 * -------------------------------------------------------- */

	/**
	 * Get a global value.
	 *
	 * @param ctx the context
	 * @param name the name of the global variable
	 * @return the value
	 */
	template <typename Type>
	inline Type getGlobal(const std::string &name)
	{
		assertBegin(m_handle.get());
		duk_get_global_string(m_handle.get(), name.c_str());
		Type value = get<Type>(-1);
		duk_pop(m_handle.get());
		assertEquals(m_handle.get());

		return value;
	}

	/**
	 * Set a global variable.
	 *
	 * @param ctx the context
	 * @param name the name of the global variable
	 * @param type the value to set
	 */
	template <typename Type>
	inline void setGlobal(const std::string &name, Type&& type)
	{
		assertBegin(m_handle.get());
		push(std::forward<Type>(type));
		duk_put_global_string(m_handle.get(), name.c_str());
		assertEquals(m_handle.get());
	}

	/**
	 * Throw an ECMAScript exception.
	 *
	 * @param ctx the context
	 * @param ex the exception
	 */
	template <typename Exception>
	void raise(const Exception &ex)
	{
		ex.create(m_handle.get());

		duk_push_string(m_handle.get(), ex.name().c_str());
		duk_put_prop_string(m_handle.get(), -2, "name");
		duk_throw(m_handle.get());
	}
};

/* ------------------------------------------------------------------
 * Exception handling
 * ------------------------------------------------------------------ */

/**
 * @class ExceptionAbstract
 * @brief Base class for standard ECMAScript exceptions
 * @warning Override the function create for your own exceptions
 */
class ExceptionAbstract {
protected:
	std::string m_name;
	std::string m_message;

public:
	/**
	 * Construct an exception of type name with the specified message.
	 *
	 * @param name the name (e.g TypeError)
	 * @param message the message
	 */
	ExceptionAbstract(std::string name, std::string message);

	/**
	 * Get the exception type name.
	 *
	 * @return the exception type
	 */
	const std::string &name() const noexcept;

	/**
	 * Create the exception on the stack.
	 *
	 * @note the default implementation search for the global variables
	 * @param ctx the context
	 */
	virtual void create(ContextPtr ctx) const noexcept;
};

/**
 * @class Error
 * @brief Base ECMAScript error class
 */
class Error : public ExceptionAbstract {
public:
	Error(std::string message);
};

/**
 * @class EvalError
 * @brief Error in eval() function
 */
class EvalError : public ExceptionAbstract {
public:
	EvalError(std::string message);
};

/**
 * @class RangeError
 * @brief Value is out of range
 */
class RangeError : public ExceptionAbstract {
public:
	RangeError(std::string message);
};

/**
 * @class ReferenceError
 * @brief Trying to use a variable that does not exist
 */
class ReferenceError : public ExceptionAbstract {
public:
	ReferenceError(std::string message);
};

/**
 * @class SyntaxError
 * @brief Syntax error in the script
 */
class SyntaxError : public ExceptionAbstract {
public:
	SyntaxError(std::string message);
};

/**
 * @class TypeError
 * @brief Invalid type given
 */
class TypeError : public ExceptionAbstract {
public:
	TypeError(std::string message);
};

/**
 * @class URIError
 * @brief URI manipulation failure
 */
class URIError : public ExceptionAbstract {
public:
	URIError(std::string message);
};

/* ------------------------------------------------------------------
 * Standard overloads for TypeInfo<T>::get
 * ------------------------------------------------------------------ */

template <>
class TypeInfo<int> {
public:
	static int get(ContextPtr ctx, duk_idx_t index)
	{
		return duk_get_int(ctx, index);
	}

	static void push(ContextPtr ctx, int value)
	{
		duk_push_int(ctx, value);
	}
};

template <>
class TypeInfo<bool> {
public:
	static bool get(Context &ctx, duk_idx_t index)
	{
		return duk_get_boolean(ctx, index);
	}

	static void push(Context &ctx, bool value)
	{
		duk_push_boolean(ctx, value);
	}
};

template <>
class TypeInfo<double> {
public:
	static double get(Context &ctx, duk_idx_t index)
	{
		return duk_get_number(ctx, index);
	}

	static void push(Context &ctx, double value)
	{
		duk_push_number(ctx, value);
	}
};

template <>
class TypeInfo<std::string> {
public:
	static std::string get(Context &ctx, duk_idx_t index)
	{
		duk_size_t size;
		const char *text = duk_get_lstring(ctx, index, &size);

		return std::string{text, size};
	}

	static void push(Context &ctx, const std::string &value)
	{
		duk_push_lstring(ctx, value.c_str(), value.length());
	}
};

template <>
class TypeInfo<const char *> {
public:
	static void push(Context &ctx, const char *value)
	{
		duk_push_string(ctx, value);
	}
};

template <>
class TypeInfo<Function> {
public:
	static void push(Context &ctx, const Function &fn)
	{
		assert(fn.function);

		duk_push_c_function(ctx, fn.function, fn.nargs);
	}
};

template <>
class TypeInfo<Object> {
public:
	static void push(Context &ctx, const Object &)
	{
		duk_push_object(ctx);
	}
};

template <>
class TypeInfo<Array> {
public:
	static void push(Context &ctx, const Array &)
	{
		duk_push_array(ctx);
	}
};

} // !js

} // !malikania

#endif // !_JS_H_

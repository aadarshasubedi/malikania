/*
 * Js.cpp -- JS API for irccd and Duktape helpers
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

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iterator>
#include <memory>
#include <unordered_map>

#include "Js.h"

using namespace std::string_literals;

namespace malikania {

namespace js {

namespace {

ErrorInfo error(ContextPtr ctx, duk_idx_t index)
{
	ErrorInfo error;

	index = duk_normalize_index(ctx, index);

	assertBegin(ctx);
	duk_get_prop_string(ctx, index, "name");
	error.name = duk_to_string(ctx, -1);
	duk_get_prop_string(ctx, index, "message");
	error.message = duk_to_string(ctx, -1);
	duk_get_prop_string(ctx, index, "fileName");
	error.fileName = duk_to_string(ctx, -1);
	duk_get_prop_string(ctx, index, "lineNumber");
	error.lineNumber = duk_to_int(ctx, -1);
	duk_get_prop_string(ctx, index, "stack");
	error.stack = duk_to_string(ctx, -1);
	duk_pop_n(ctx, 5);
	assertEquals(ctx);

	return error;
}

} // !namespace

/* --------------------------------------------------------
 * Basic functions
 * -------------------------------------------------------- */

duk_int_t Context::type(duk_idx_t index)
{
	return duk_get_type(m_handle.get(), index);
}

int Context::top()
{
	return duk_get_top(m_handle.get());
}

void Context::pop(int count)
{
	assert(count > 0);

	duk_pop_n(m_handle.get(), count);
}

duk_bool_t Context::instanceof(duk_idx_t idx1, duk_idx_t idx2)
{
	return duk_instanceof(m_handle.get(), idx1, idx2);
}

void Context::call(duk_idx_t nargs)
{
	duk_call(m_handle.get(), nargs);
}

void Context::pcall(duk_idx_t nargs)
{
	if (duk_pcall(m_handle.get(), nargs) != 0) {
		ErrorInfo info = error(m_handle.get(), -1);
		pop();

		throw info;
	}
}

/* ------------------------------------------------------------------
 * Eval functions
 * ------------------------------------------------------------------ */

void Context::eval()
{
	duk_eval(m_handle.get());
}

void Context::evalString(const std::string &name)
{
	duk_eval_string(m_handle.get(), name.c_str());
}

void Context::evalFile(const std::string &file)
{
	duk_eval_file(m_handle.get(), file.c_str());
}

void Context::peval()
{
	if (duk_peval(m_handle.get()) != 0) {
		throw error(m_handle.get(), -1);
	}
}

void Context::pevalString(const std::string &script)
{
	if (duk_peval_string(m_handle.get(), script.c_str()) != 0) {
		ErrorInfo info = error(m_handle.get(), -1);
		pop();

		throw info;
	}
}

void Context::pevalFile(const std::string &file)
{
	if (duk_peval_file(m_handle.get(), file.c_str()) != 0) {
		ErrorInfo info = error(m_handle.get(), -1);
		pop();

		throw info;
	}
}

/* ------------------------------------------------------------------
 * ExceptionAbstract class
 * ------------------------------------------------------------------ */

ExceptionAbstract::ExceptionAbstract(std::string name, std::string message)
	: m_name(std::move(name))
	, m_message(std::move(message))
{
}

const std::string &ExceptionAbstract::name() const noexcept
{
	return m_name;
}

void ExceptionAbstract::create(ContextPtr ctx) const noexcept
{
	duk_get_global_string(ctx, m_name.c_str());
	duk_push_string(ctx, m_message.c_str());
	duk_new(ctx, 1);
}

/* ------------------------------------------------------------------
 * Error class
 * ------------------------------------------------------------------ */

Error::Error(std::string message)
	: ExceptionAbstract("Error", std::move(message))
{
}

/* ------------------------------------------------------------------
 * EvalError class
 * ------------------------------------------------------------------ */

EvalError::EvalError(std::string message)
	: ExceptionAbstract("EvalError", std::move(message))
{
}

/* ------------------------------------------------------------------
 * RangeError class
 * ------------------------------------------------------------------ */

RangeError::RangeError(std::string message)
	: ExceptionAbstract("RangeError", std::move(message))
{
}

/* ------------------------------------------------------------------
 * ReferenceError class
 * ------------------------------------------------------------------ */

ReferenceError::ReferenceError(std::string message)
	: ExceptionAbstract("ReferenceError", std::move(message))
{
}

/* ------------------------------------------------------------------
 * SyntaxError class
 * ------------------------------------------------------------------ */

SyntaxError::SyntaxError(std::string message)
	: ExceptionAbstract("SyntaxError", std::move(message))
{
}

/* ------------------------------------------------------------------
 * TypeError class
 * ------------------------------------------------------------------ */

TypeError::TypeError(std::string message)
	: ExceptionAbstract("TypeError", std::move(message))
{
}

/* ------------------------------------------------------------------
 * URIError class
 * ------------------------------------------------------------------ */

URIError::URIError(std::string message)
	: ExceptionAbstract("URIError", std::move(message))
{
}

} // !js

} // !malikania

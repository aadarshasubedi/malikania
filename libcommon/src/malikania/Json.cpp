/*
 * Json.cpp -- C++14 JSON manipulation using jansson parser
 *
 * Copyright (c) 2013-2015 David Demelier <markand@malikania.fr>
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

#include "Json.h"

#include <jansson.h>

namespace malikania {

namespace json {

namespace {

void readObject(Value &parent, json_t *object);
void readArray(Value &parent, json_t *array);

Value readValue(json_t *v)
{
	if (json_is_null(v)) {
		return Value{nullptr};
	}
	if (json_is_string(v)) {
		return Value{json_string_value(v)};
	}
	if (json_is_real(v)) {
		return Value{json_number_value(v)};
	}
	if (json_is_integer(v)) {
		return Value{static_cast<int>(json_integer_value(v))};
	}
	if (json_is_boolean(v)) {
		return Value{json_boolean_value(v)};
	}
	if (json_is_object(v)) {
		Object object;

		readObject(object, v);

		return object;
	}
	if (json_is_array(v)) {
		Array array;

		readArray(array, v);

		return array;
	}

	return Value{};
}

void readObject(Value &parent, json_t *object)
{
	const char *key;
	json_t *value;

	json_object_foreach(object, key, value) {
		static_cast<Object &>(parent).insert(key, readValue(value));
	}
}

void readArray(Value &parent, json_t *array)
{
	size_t index;
	json_t *value;

	json_array_foreach(array, index, value) {
		static_cast<Array &>(parent).append(readValue(value));
	}
}

template <typename Func, typename... Args>
Value convert(Func fn, Args&&... args)
{
	json_error_t error;
	json_t *json = fn(std::forward<Args>(args)..., &error);

	if (json == nullptr) {
		throw Error{error.text, error.source, error.line, error.column, error.position};
	}

	Value value;

	if (json_is_object(json)) {
		value = Object{};
		readObject(value, json);
	} else {
		value = Array{};
		readArray(value, json);
	}

	json_decref(json);

	return value;
}

} // !namespace

bool Value::toBool() const noexcept
{
	if (m_type != Type::Boolean) {
		return false;
	}

	return m_boolean;
}

double Value::toReal() const noexcept
{
	if (m_type != Type::Real) {
		return 0;
	}

	return m_number;
}

int Value::toInt() const noexcept
{
	if (m_type != Type::Int) {
		return 0;
	}

	return m_integer;
}

std::string Value::toString() const noexcept
{
	if (m_type != Type::String) {
		return "";
	}

	return m_string;
}

Object Value::toObject() const noexcept
{
	if (m_type != Type::Object) {
		return Object{};
	}

	return Object(*this);
}

Array Value::toArray() const noexcept
{
	if (m_type != Type::Array) {
		return Array{};
	}

	return Array(*this);
}

Document::Document(Buffer buffer)
{
	m_value = convert(json_loads, buffer.text.c_str(), 0);
}

Document::Document(File file)
{
	m_value = convert(json_load_file, file.path.c_str(), 0);
}

std::string escape(std::string value) noexcept
{
	for (auto it = value.begin(); it != value.end(); ++it) {
		switch (*it) {
		case '\\':
		case '/':
		case '"':
			it = value.insert(it, '\\');
			it++;
			break;
		case '\b':
			value.replace(it, it + 1, "\\b");
			it += 1;
			break;
		case '\f':
			value.replace(it, it + 1, "\\f");
			it += 1;
			break;
		case '\n':
			value.replace(it, it + 1, "\\n");
			it += 1;
			break;
		case '\r':
			value.replace(it, it + 1, "\\r");
			it += 1;
			break;
		case '\t':
			value.replace(it, it + 1, "\\t");
			it += 1;
			break;
		default:
			break;
		}
	}

	return value;
}

} // !json

} // !malikania

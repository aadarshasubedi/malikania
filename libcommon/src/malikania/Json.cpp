/*
 * Json.cpp -- jansson C++11 wrapper
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

#include "Json.h"

namespace malikania {

/* --------------------------------------------------------
 * JsonValue implementation
 * -------------------------------------------------------- */

JsonValue::JsonValue(const JsonValue &value)
	: m_handle{json_deep_copy(value.m_handle.get()), json_decref}
{
}

JsonValue &JsonValue::operator=(const JsonValue &value)
{
	m_handle = {json_deep_copy(value.m_handle.get()), json_decref};
}

JsonValue::JsonValue(json_t *json)
	: m_handle{json, json_decref}
{
}

JsonValue::JsonValue()
	: m_handle{json_null(), json_decref}
{
}

JsonValue::JsonValue(bool value)
	: m_handle{json_boolean(value), json_decref}
{
}

JsonValue::JsonValue(int value)
	: m_handle{json_integer(value), json_decref}
{
}

JsonValue::JsonValue(double value)
	: m_handle{json_real(value), json_decref}
{
}

JsonValue::JsonValue(std::string value)
	: m_handle{json_string(value.c_str()), json_decref}
{
}

JsonValue::JsonValue(const char *value)
	: m_handle{json_string(value), json_decref}
{
}

JsonType JsonValue::typeOf() const
{
	return static_cast<JsonType>(json_typeof(m_handle.get()));
}

bool JsonValue::isObject() const
{
	return json_is_object(m_handle.get()) != 0;
}

bool JsonValue::isArray() const
{
	return json_is_array(m_handle.get()) != 0;
}

bool JsonValue::isString() const
{
	return json_is_string(m_handle.get()) != 0;
}

bool JsonValue::isReal() const
{
	return json_is_real(m_handle.get()) != 0;
}

bool JsonValue::isTrue() const
{
	return json_is_true(m_handle.get()) != 0;
}

bool JsonValue::isFalse() const
{
	return json_is_false(m_handle.get()) != 0;
}

bool JsonValue::isNull() const
{
	return json_is_null(m_handle.get()) != 0;
}

bool JsonValue::isNumber() const
{
	return json_is_number(m_handle.get()) != 0;
}

bool JsonValue::isInteger() const
{
	return json_is_integer(m_handle.get()) != 0;
}

bool JsonValue::isBoolean() const
{
	return json_is_boolean(m_handle.get()) != 0;
}

std::string JsonValue::toString() const
{
	auto value = json_string_value(m_handle.get());

	return (value == nullptr) ? "" : value;
}

int JsonValue::toInteger() const noexcept
{
	return json_integer_value(m_handle.get());
}

double JsonValue::toReal() const noexcept
{
	return json_real_value(m_handle.get());
}

JsonObject JsonValue::toObject() const
{
	json_incref(m_handle.get());

	return JsonObject{m_handle.get()};
}

JsonArray JsonValue::toArray() const
{
	json_incref(m_handle.get());

	return JsonArray{m_handle.get()};
}

/* --------------------------------------------------------
 * JsonArray
 * -------------------------------------------------------- */

JsonArray::JsonArray()
	: JsonValue{json_array()}
{
}

unsigned JsonArray::size() const noexcept
{
	return json_array_size(m_handle.get());
}

void JsonArray::push(const JsonValue &value)
{
	json_array_insert(m_handle.get(), 0, value.m_handle.get());
}

void JsonArray::append(const JsonValue &value)
{
	json_array_append(m_handle.get(), value.m_handle.get());
}

void JsonArray::insert(const JsonValue &value, int index)
{
	json_array_insert(m_handle.get(), index, value.m_handle.get());
}

JsonValue JsonArray::operator[](int index) const
{
	if (typeOf() != JsonType::Array)
		throw JsonError{"not an array"};

	auto value = json_array_get(m_handle.get(), index);

	if (value == nullptr)
		throw JsonError{"index out of bounds"};

	json_incref(value);

	return JsonValue{value};
}

/* --------------------------------------------------------
 * JsonObject
 * -------------------------------------------------------- */

JsonObject::JsonObject()
	: JsonValue{json_object()}
{
}

JsonValue JsonObject::operator[](const std::string &name) const
{
	if (typeOf() != JsonType::Object)
		throw JsonError{"not an object"};

	auto value = json_object_get(m_handle.get(), name.c_str());

	if (value == nullptr)
		throw JsonError{"key " + name + +" not found"};

	json_incref(value);

	return JsonValue{value};
}

void JsonObject::set(const std::string &key, const JsonValue &value)
{
	json_object_set(m_handle.get(), key.c_str(), value.m_handle.get());
}

/* --------------------------------------------------------
 * JsonReaderFile
 * -------------------------------------------------------- */

JsonReaderFile::JsonReaderFile(std::string path)
	: m_path{std::move(path)}
{
}

JsonValue JsonReaderFile::read()
{
	json_error_t error;
	json_t *handle = json_load_file(m_path.c_str(), 0, &error);

	if (handle == nullptr)
		throw JsonError{error};

	return JsonValue{handle};
}

/* --------------------------------------------------------
 * JsonWriterFile
 * -------------------------------------------------------- */

JsonWriterFile::JsonWriterFile(std::string path)
	: m_path{std::move(path)}
{
}

void JsonWriterFile::write(const JsonValue &value)
{
	if (json_dump_file(value, m_path.c_str(), 0) < 0)
		throw JsonError{"Failed to write file: " + m_path};
}

} // !malikania

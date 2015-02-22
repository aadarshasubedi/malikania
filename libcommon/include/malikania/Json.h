/*
 * Json.h -- jansson C++11 wrapper
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

#ifndef _JSON_H_
#define _JSON_H_

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <jansson.h>

/**
 * @file Json.h
 * @brief A jansson C++ modern wrapper
 *
 * Because of the Jansson implementation, all these classes are implicitly
 * shared.
 *
 * This means that you can't set any value to an existing value as it would
 * change a value which may be used somewhere else, instead you must set
 * or replace elements in JsonObject and JsonArray respectively.
 *
 * However, copy constructors are implemented as deep copy so take care of
 * not copying values mistakenly.
 */

namespace malikania {

/**
 * @class JsonType
 * @brief Json value type
 */
enum class JsonType {
	Object = JSON_OBJECT,		//!< Object
	Array = JSON_ARRAY,		//!< Array
	String = JSON_STRING,		//!< String
	Integer = JSON_INTEGER,		//!< Integer
	Real = JSON_REAL,		//!< Floating point
	True = JSON_TRUE,		//!< Boolean true
	False = JSON_FALSE,		//!< Boolean false
	Null = JSON_NULL		//!< Empty or null
};

class JsonObject;
class JsonArray;

/**
 * @class JsonValue
 * @brief Encapsulate any JSON value
 */
class JsonValue {
public:
	using Handle = std::unique_ptr<json_t, void (*)(json_t *)>;

	friend class JsonObject;
	friend class JsonArray;
	friend class JsonDocument;

protected:
	/**
	 * The unique_ptr handle of json_t, will automatically decrease
	 * the reference count in its deleter.
	 */
	Handle m_handle;

public:
	/**
	 * Create a JsonValue from a native Jansson type. This function
	 * will increment the json_t reference count.
	 *
	 * @param json the value
	 */
	JsonValue(json_t *json);

	/**
	 * Deep copy of that element.
	 *
	 * @param value the other value
	 */
	JsonValue(const JsonValue &value);

	/**
	 * Assign a deep copy of the other element.
	 *
	 * @return *this
	 */
	JsonValue &operator=(const JsonValue &);

	/**
	 * Default move constructor.
	 *
	 * @param other the other value
	 */
	JsonValue(JsonValue &&other) = default;

	/**
	 * Default move assignment.
	 *
	 * @param other the other value
	 */
	JsonValue &operator=(JsonValue &&) = default;

	/**
	 * Create an empty value (JsonType::Null).
	 */
	JsonValue();

	/**
	 * Create a boolean value.
	 *
	 * @param value the value
	 */
	JsonValue(bool value);

	/**
	 * Create a integer value (JsonType::Integer).
	 *
	 * @param value the value
	 */
	JsonValue(int value);

	/**
	 * Create a real value (JsonType::Real).
	 *
	 * @param value the value
	 */
	JsonValue(double value);

	/**
	 * Create a string value (JsonType::String).
	 * @param value
	 */
	JsonValue(std::string value);

	/**
	 * Create from a C string (JsonType::String).
	 *
	 * @param value the string
	 */
	JsonValue(const char *value);

	/**
	 * Create from a string literal (JsonType::String).
	 *
	 * @param value the value
	 */
	template <size_t Size>
	inline JsonValue(char (&value)[Size])
		: m_handle{json_string(value), json_decref}
	{
	}

	/**
	 * Default destructor.
	 */
	virtual ~JsonValue() = default;

	/**
	 * Get the type of value.
	 *
	 * @return the type
	 */
	JsonType typeOf() const;

	/**
	 * Tells if the json value is an JSON_OBJECT.
	 *
	 * @return true or false
	 */
	bool isObject() const;

	/**
	 * Tells if the json value is an JSON_ARRAY.
	 *
	 * @return true or false
	 */
	bool isArray() const;

	/**
	 * Tells if the json value is an JSON_STRING.
	 *
	 * @return true or false
	 */
	bool isString() const;

	/**
	 * Tells if the json value is an JSON_REAL.
	 *
	 * @return true or false
	 */
	bool isReal() const;

	/**
	 * Tells if the json value is an JSON_TRUE.
	 *
	 * @return true or false
	 */
	bool isTrue() const;

	/**
	 * Tells if the json value is an JSON_FALSE.
	 *
	 * @return true or false
	 */
	bool isFalse() const;

	/**
	 * Tells if the json value is an JSON_NULL.
	 *
	 * @return true or false
	 */
	bool isNull() const;

	/**
	 * Tells if the json value is an JSON_INTEGER or JSON_REAL.
	 *
	 * @return true or false
	 */
	bool isNumber() const;

	/**
	 * Tells if the json value is an JSON_INTEGER.
	 *
	 * @return true or false
	 */
	bool isInteger() const;

	/**
	 * Tells if the json value is an JSON_TRUE or JSON_FALSE.
	 *
	 * @return true or false
	 */
	bool isBoolean() const;

	/**
	 * Get the string value.
	 *
	 * @return the string
	 */
	std::string toString() const;

	/**
	 * Get the integer value.
	 *
	 * @return the value or 0
	 */
	int toInteger() const noexcept;

	/**
	 * Get the real value.
	 *
	 * @return the value or 0
	 */
	double toReal() const noexcept;

	/**
	 * Convert to object.
	 *
	 * @return an object
	 */
	JsonObject toObject() const;

	/**
	 * Convert to array.
	 *
	 * @return an array
	 */
	JsonArray toArray() const;

	/**
	 * Convert to native Jansson type.
	 *
	 * You should not call json_incref or json_decref on it as it is
	 * automatically done.
	 *
	 * @return the json_t handle
	 * @warning use this function with care
	 */
	inline operator json_t *() noexcept
	{
		return m_handle.get();
	}

	/**
	 * Overloaded function.
	 *
	 * @return the json_t handle
	 */
	inline operator const json_t *() const noexcept
	{
		return m_handle.get();
	}
};

/**
 * @class JsonArray
 * @brief Manipulate JSON arrays
 */
class JsonArray final : public JsonValue {
public:
	using JsonValue::JsonValue;

	/**
	 * Create an empty array.
	 */
	JsonArray();

	/**
	 * Get the number of values in the array
	 *
	 * @return the number or 0
	 */
	unsigned size() const noexcept;

	/**
	 * Insert the value at the beginning.
	 *
	 * @param value the value
	 */
	void push(const JsonValue &value);

	/**
	 * Insert a copy of the value at the end.
	 *
	 * @param value the value to insert
	 */
	void append(const JsonValue &value);

	/**
	 * Insert a copy of the value at the specified index.
	 *
	 * @param value the value to insert
	 * @param index the position
	 */
	void insert(const JsonValue &value, int index);

	/**
	 * Get the value at the specified index.
	 *
	 * @param index the position
	 * @return the value
	 * @throw std::invalid_argument on error
	 * @throw std::out_of_range on bad arguments
	 */
	JsonValue operator[](int index) const;

	/**
	 * Iterate over the array value (not recursively).
	 *
	 * The function must have the following signature:
	 *    void f(int, const JsonValue &)
	 *
	 * @param function the function to call
	 */
	template <typename Func>
	void forAll(Func function) const
	{
		json_t *value;
		size_t index;

		json_array_foreach(m_handle.get(), index, value) {
			json_incref(value);
			function(static_cast<int>(index), JsonValue{value});
		}
	}
};

/**
 * @class JsonObject
 * @brief Object wrapper
 */
class JsonObject final : public JsonValue {
public:
	using JsonValue::JsonValue;

	/**
	 * Create empty object.
	 */
	JsonObject();

	/**
	 * Get the value at the specified key. The returned value is a borrowed reference,
	 *
	 * @param key the key
	 * @return the value
	 * @throw std::invalid_argument on error
	 * @throw std::out_of_range on bad arguments
	 */
	JsonValue operator[](const std::string &key) const;

	/**
	 * Set the value as key in the object.
	 *
	 * @param key the key
	 * @param value the value
	 */
	void set(const std::string &key, const JsonValue &value);

	/**
	 * Iterate over the object keys (not recursively).
	 *
	 * The function must have the following signature:
	 *    void f(const std::string &key, const JsonValue &)
	 *
	 * @param function the function to call
	 */
	template <typename Func>
	void forAll(Func function) const
	{
		json_t *value;
		const char *key;

		json_object_foreach(m_handle.get(), key, value) {
			json_incref(value);
			function(std::string(key), JsonValue{value});
		}
	}
};

/**
 * @class JsonError
 * @brief Error thrown for any error
 */
class JsonError final : public std::exception {
private:
	friend class Json;

	std::string	m_text;
	std::string	m_source;
	int		m_line{};
	int		m_column{};
	int		m_position{};

public:
	/**
	 * Custom error with no line, no column and no position.
	 *
	 * @param error the error message
	 */
	inline JsonError(std::string error)
		: m_text{std::move(error)}
	{
	}

	/**
	 * Error from a json_error_t.
	 *
	 * @param error the error
	 */
	inline JsonError(const json_error_t &error)
		: m_text{error.text}
		, m_source{error.source}
		, m_line{error.line}
		, m_column{error.column}
		, m_position{error.position}
	{
	}

	/**
	 * Get the error message.
	 *
	 * @return the message
	 */
	const char *what() const noexcept override
	{
		return m_text.c_str();
	}

	/**
	 * Get the text message.
	 *
	 * @return the text
	 */
	inline const std::string &text() const noexcept
	{
		return m_text;
	}

	/**
	 * Get the source.
	 *
	 * @return the source
	 */
	inline const std::string &source() const noexcept
	{
		return m_source;
	}

	/**
	 * Get the line.
	 *
	 * @return the line
	 */
	inline int line() const noexcept
	{
		return m_line;
	}

	/**
	 * Get the column.
	 *
	 * @return the column
	 */
	inline int column() const noexcept
	{
		return m_column;
	}

	/**
	 * Get the position.
	 *
	 * @return the position
	 */
	inline int position() const noexcept
	{
		return m_position;
	}
};

/**
 * @class JsonReader
 * @brief Base class for JSON reading
 */
class JsonReader {
public:
	/**
	 * Default constructor.
	 */
	JsonReader() = default;

	/**
	 * Default destructor.
	 */
	virtual ~JsonReader() = default;

	/**
	 * Read the source and return a value.
	 *
	 * @return a value
	 */
	virtual JsonValue read() = 0;
};

/**
 * @class JsonWriter
 * @brief Base class for writing
 */
class JsonWriter {
public:
	/**
	 * Default destructor.
	 */
	JsonWriter() = default;

	/**
	 * Default destructor.
	 */
	virtual ~JsonWriter() = default;

	/**
	 * Write to the source the value.
	 *
	 * @param value the value
	 */
	virtual void write(const JsonValue &value) = 0;
};

/**
 * @class JsonReaderFile
 * @brief Read JSON data from a file
 */
class JsonReaderFile final : public JsonReader {
private:
	std::string m_path;

public:
	/**
	 * Construct a JsonReader to a file. This function does not open
	 * the file immediately.
	 *
	 * @param path the path
	 */
	JsonReaderFile(std::string path);

	/**
	 * Read the file and extract a JsonValue.
	 *
	 * @return a JsonValue (which will be JsonType::Object or JsonType::Array)
	 * @throw JsonError on errors
	 */
	JsonValue read() override;
};

/**
 * @class JsonWriterFile
 * @brief Write JSON data to file
 */
class JsonWriterFile final : public JsonWriter {
private:
	std::string m_path;

public:
	/**
	 * Construct a JsonReader to a file. This function does not open
	 * the file immediately.
	 *
	 * @param path
	 */
	JsonWriterFile(std::string path);

	/**
	 * Write to the file.
	 *
	 * @param value the value
	 * @throw JsonError on errors
	 */
	void write(const JsonValue &value) override;
};

} // !malikania

#endif // !_JSON_H_

/*
 * Json.h -- C++14 JSON manipulation using jansson parser
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

#ifndef _JSON_H_
#define _JSON_H_

/**
 * @file Json.h
 * @brief Jansson C++14 wrapper
 *
 * These classes can be used to build or parse JSON documents using jansson library. It is designed to be safe
 * and explicit. It does not implement implicit sharing like jansson so when you access (e.g. Value::toObject) values
 * you get real copies, thus when you read big documents it can has a performance cost.
 */

#include <exception>
#include <initializer_list>
#include <map>
#include <string>
#include <utility>
#include <deque>

namespace malikania {

/**
 * Json namespace.
 */
namespace json {

/**
 * @enum Type
 * @brief Type of Value.
 */
enum class Type {
	Array,		//!< Value is an array []
	Boolean,	//!< Value is boolean
	Int,		//!< Value is integer
	Real,		//!< Value is float
	Object,		//!< Value is object {}
	String,		//!< Value is unicode string
	Null		//!< Value is defined to null
};

/**
 * @class Error
 * @brief Error description.
 */
class Error : public std::exception {
private:
	std::string m_text;
	std::string m_source;
	int m_line;
	int m_column;
	int m_position;

public:
	/**
	 * Create the error.
	 *
	 * @param ptext the text message
	 * @param psource the source (e.g. file name)
	 * @param pline the line number
	 * @param pcolumn the column number
	 * @param pposition the position
	 */
	inline Error(std::string text, std::string source, int line, int column, int position) noexcept
		: m_text{std::move(text)}
		, m_source{std::move(source)}
		, m_line{line}
		, m_column{column}
		, m_position{position}
	{
	}

	inline const std::string &text() const noexcept
	{
		return m_text;
	}

	inline const std::string &source() const noexcept
	{
		return m_source;
	}

	inline int line() const noexcept
	{
		return m_line;
	}

	inline int column() const noexcept
	{
		return m_column;
	}

	inline int position() const noexcept
	{
		return m_position;
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
};

class Array;
class Object;

/**
 * @class Value
 * @brief Generic JSON value wrapper.
 */
class Value {
protected:
	/**
	 * Type of value.
	 */
	Type m_type;

	union {
		double m_number;
		bool m_boolean;
		int m_integer;
		std::string m_string;
		std::deque<Value> m_array;
		std::map<std::string, Value> m_map;
	};

public:
	/**
	 * Construct a null value.
	 */
	inline Value() noexcept
		: m_type{Type::Null}
	{
	}

	/**
	 * Construct a null value.
	 */
	inline Value(std::nullptr_t) noexcept
		: m_type{Type::Null}
	{
	}

	/**
	 * Construct a boolean value.
	 *
	 * @param value the boolean value
	 */
	inline Value(bool value) noexcept
		: m_type{Type::Boolean}
		, m_boolean{value}
	{
	}

	/**
	 * Create value from integer.
	 *
	 * @param value the value
	 */
	inline Value(int value) noexcept
		: m_type{Type::Int}
		, m_integer{value}
	{
	}

	/**
	 * Construct a value from a C-string.
	 *
	 * @param value the C-string
	 */
	inline Value(const char *value)
		: m_type{Type::String}
	{
		new (&m_string) std::string{value};
	}

	/**
	 * Construct a number value.
	 *
	 * @param value the real value
	 */
	inline Value(double value) noexcept
		: m_type{Type::Real}
		, m_number{value}
	{
	}

	/**
	 * Construct a string value.
	 *
	 * @param value the string
	 */
	inline Value(std::string value) noexcept
		: m_type{Type::String}
	{
		new (&m_string) std::string{std::move(value)};
	}

	/**
	 * Move constructor.
	 *
	 * @param other the value to move from
	 */
	inline Value(Value &&other)
	{
		m_type = other.m_type;

		switch (m_type) {
		case Type::String:
			new (&m_string) std::string();
			m_string = std::move(other.m_string);
			break;
		case Type::Real:
			m_number = other.m_number;
			break;
		case Type::Int:
			m_integer = other.m_integer;
			break;
		case Type::Boolean:
			m_boolean = other.m_boolean;
			break;
		case Type::Object:
			new (&m_map) std::map<std::string, Value>();
			m_map = std::move(other.m_map);
			break;
		case Type::Array:
			new (&m_array) std::deque<Value>();
			m_array = std::move(other.m_array);
			break;
		default:
			break;
		}
	}

	/**
	 * Copy constructor.
	 *
	 * @param other the value to copy from
	 */
	inline Value(const Value &other)
	{
		m_type = other.m_type;

		switch (m_type) {
		case Type::String:
			new (&m_string) std::string();
			m_string = other.m_string;
			break;
		case Type::Real:
			m_number = other.m_number;
			break;
		case Type::Int:
			m_integer = other.m_integer;
			break;
		case Type::Boolean:
			m_boolean = other.m_boolean;
			break;
		case Type::Object:
			new (&m_map) std::map<std::string, Value>();
			m_map = other.m_map;
			break;
		case Type::Array:
			new (&m_array) std::deque<Value>();
			m_array = other.m_array;
			break;
		default:
			break;
		}
	}

	/**
	 * Copy operator.
	 *
	 * @param other the value to copy from
	 * @return *this
	 */
	inline Value &operator=(const Value &other)
	{
		m_type = other.m_type;

		switch (m_type) {
		case Type::String:
			new (&m_string) std::string();
			m_string = other.m_string;
			break;
		case Type::Real:
			m_number = other.m_number;
			break;
		case Type::Int:
			m_integer = other.m_integer;
			break;
		case Type::Boolean:
			m_boolean = other.m_boolean;
			break;
		case Type::Object:
			new (&m_map) std::map<std::string, Value>();
			m_map = other.m_map;
			break;
		case Type::Array:
			new (&m_array) std::deque<Value>();
			m_array = other.m_array;
			break;
		default:
			break;
		}

		return *this;
	}

	/**
	 * Move operator.
	 *
	 * @param other the value to move from
	 */
	inline Value &operator=(Value &&other)
	{
		m_type = other.m_type;

		switch (m_type) {
		case Type::String:
			new (&m_string) std::string();
			m_string = std::move(other.m_string);
			break;
		case Type::Real:
			m_number = other.m_number;
			break;
		case Type::Int:
			m_integer = other.m_integer;
			break;
		case Type::Boolean:
			m_boolean = other.m_boolean;
			break;
		case Type::Object:
			new (&m_map) std::map<std::string, Value>();
			m_map = std::move(other.m_map);
			break;
		case Type::Array:
			new (&m_array) std::deque<Value>();
			m_array = std::move(other.m_array);
			break;
		default:
			break;
		}

		return *this;
	}

	/**
	 * Destructor.
	 */
	inline ~Value()
	{
		switch (m_type) {
		case Type::String:
			m_string.~basic_string();
			break;
		case Type::Object:
			m_map.~map<std::string, Value>();
			break;
		case Type::Array:
			m_array.~deque<Value>();
			break;
		default:
			break;
		}
	}

	/**
	 * Get the value type.
	 *
	 * @return the type
	 */
	inline Type typeOf() const noexcept
	{
		return m_type;
	}

	/**
	 * Get the value as boolean.
	 *
	 * @return the value or false if not a boolean
	 */
	bool toBool() const noexcept;

	/**
	 * Get the value as integer.
	 *
	 * @return the value or 0 if not a integer
	 */
	int toInt() const noexcept;

	/**
	 * Get the value as real.
	 *
	 * @return the value or 0 if not a real
	 */
	double toReal() const noexcept;

	/**
	 * Get the value as string.
	 *
	 * @return the value or empty stirng if not a string
	 */
	std::string toString() const noexcept;

	/**
	 * Convert the value to object.
	 *
	 * @return an object or empty if not an object
	 * @note the returned object is a copy, modifying it won't modify this value
	 */
	Object toObject() const noexcept;

	/**
	 * Convert the value to array.
	 *
	 * @return an array or empty if not an array
	 * @note the returned array is a copy, modifying it won't modify this value
	 */
	Array toArray() const noexcept;

	/**
	 * Check if the value is boolean type.
	 *
	 * @return true if boolean
	 */
	inline bool isBool() const noexcept
	{
		return m_type == Type::Boolean;
	}

	/**
	 * Check if the value is integer type.
	 *
	 * @return true if integer
	 */
	inline bool isInt() const noexcept
	{
		return m_type == Type::Int;
	}

	/**
	 * Check if the value is object type.
	 *
	 * @return true if object
	 */
	inline bool isObject() const noexcept
	{
		return m_type == Type::Object;
	}

	/**
	 * Check if the value is array type.
	 *
	 * @return true if array
	 */
	inline bool isArray() const noexcept
	{
		return m_type == Type::Array;
	}

	/**
	 * Check if the value is integer or real type.
	 *
	 * @return true if integer or real
	 * @see toInt
	 * @see toReal
	 */
	inline bool isNumber() const noexcept
	{
		return m_type == Type::Real || m_type == Type::Int;
	}

	/**
	 * Check if the value is real type.
	 *
	 * @return true if real
	 */
	inline bool isReal() const noexcept
	{
		return m_type == Type::Real;
	}

	/**
	 * Check if the value is null type.
	 *
	 * @return true if null
	 */
	inline bool isNull() const noexcept
	{
		return m_type == Type::Null;
	}

	/**
	 * Check if the value is string type.
	 *
	 * @return true if string
	 */
	inline bool isString() const noexcept
	{
		return m_type == Type::String;
	}
};

/**
 * @class Array
 * @brief Array definition.
 */
class Array : public Value {
public:
	/**
	 * Construct an empty array.
	 */
	inline Array()
	{
		m_type = Type::Array;
		new (&m_array) std::deque<Value>();
	}

	/**
	 * Copy constructor from value for safe casts.
	 *
	 * @param v the value to copy from
	 */
	inline Array(const Value &v)
		: Value{v}
	{
	}

	/**
	 * Move constructor from value for safe casts.
	 *
	 * @param v the value to move from
	 */
	inline Array(Value &&v)
		: Value{std::move(v)}
	{
	}

	/**
	 * Construct an array from a std::initializer_list.
	 *
	 * @param values the list of values
	 */
	inline Array(std::initializer_list<Value> values)
	{
		m_type = Type::Array;
		new (&m_array) std::deque<Value>(values.begin(), values.end());
	}

	/**
	 * Get non-const iterator to the beginning.
	 *
	 * @return the iterator
	 */
	inline auto begin() noexcept
	{
		return m_array.begin();
	}

	/**
	 * Get a const iterator to the beginning.
	 *
	 * @return the iterator
	 */
	inline auto begin() const noexcept
	{
		return m_array.begin();
	}

	/**
	 * Get a const iterator to the beginning.
	 *
	 * @return the iterator
	 */
	inline auto cbegin() const noexcept
	{
		return m_array.cbegin();
	}

	/**
	 * Get a non-const iterator to the end.
	 *
	 * @return the iterator
	 */
	inline auto end() noexcept
	{
		return m_array.end();
	}

	/**
	 * Get a const iterator to the end.
	 *
	 * @return the iterator
	 */
	inline auto end() const noexcept
	{
		return m_array.end();
	}

	/**
	 * Get a const iterator to the end.
	 *
	 * @return the iterator
	 */
	inline auto cend() const noexcept
	{
		return m_array.cend();
	}

	/**
	 * Get the value at the specified position or the defaultValue if position is out of bounds.
	 *
	 * @param position the position
	 * @param defaultValue the value replacement
	 * @return the value or defaultValue
	 */
	template <typename V>
	inline Value valueOr(unsigned position, V &&defaultValue) const
	{
		if (position >= m_array.size()) {
			return defaultValue;
		}

		return m_array[position];
	}

	/**
	 * Get a value at the specified index.
	 *
	 * @param position the position
	 * @return the value
	 * @throw std::out_of_range if out of bounds
	 */
	inline const Value &at(unsigned position) const
	{
		return m_array.at(position);
	}

	/**
	 * Get a value at the specified index.
	 *
	 * @param position the position
	 * @return the value
	 * @throw std::out_of_range if out of bounds
	 */
	inline Value &at(unsigned position)
	{
		return m_array.at(position);
	}

	/**
	 * Get a value at the specified index.
	 *
	 * @param position the position
	 * @return the value
	 */
	inline const Value &operator[](unsigned position) const
	{
		return m_array[position];
	}

	/**
	 * Overloaded function.
	 *
	 * @param position the position
	 * @return the value
	 */
	inline Value &operator[](unsigned position)
	{
		return m_array[position];
	}

	/**
	 * Push a value to the beginning of the array.
	 *
	 * @param value the value to push
	 */
	template <typename T>
	inline void push(T &&value)
	{
		m_array.push_front(std::forward<T>(value));
	}

	/**
	 * Insert a value at the specified position.
	 *
	 * @param position the position
	 * @param value the value to push
	 */
	template <typename T>
	inline void insert(unsigned position, T &&value)
	{
		m_array.insert(m_array.begin() + position, std::forward<T>(value));
	}

	/**
	 * Add a new value.
	 *
	 * @param value the value to append
	 */
	template <typename T>
	inline void append(T &&value)
	{
		m_array.push_back(std::forward<T>(value));
	}

	/**
	 * Get the array size.
	 *
	 * @return the size
	 */
	inline unsigned size() const noexcept
	{
		return m_array.size();
	}

	/**
	 * Remove all the values.
	 */
	inline void clear()
	{
		m_array.clear();
	}

	/**
	 * Remove a value at the specified position.
	 *
	 * @param position the position
	 */
	inline void erase(unsigned position)
	{
		m_array.erase(m_array.begin() + position);
	}
};

/**
 * @class Object
 * @brief Object definition.
 */
class Object : public Value {
public:
	/**
	 * Construct an empty object.
	 */
	Object()
	{
		m_type = Type::Object;
		new (&m_map) std::map<std::string, Value>();
	}

	/**
	 * Copy constructor from value for safe casts.
	 *
	 * @param v the value to copy from
	 */
	Object(const Value &v)
		: Value{v}
	{
	}

	/**
	 * Move constructor from value for safe casts.
	 *
	 * @param v the value to move from
	 */
	Object(Value &&v)
		: Value{std::move(v)}
	{
	}

	/**
	 * Create an object from the initializer_list of key-value pairs.
	 *
	 * @param values the values
	 */
	Object(std::initializer_list<std::pair<std::string, Value>> values)
	{
		m_type = Type::Object;
		new (&m_map) std::map<std::string, Value>(values.begin(), values.end());
	}

	/**
	 * Get non-const iterator to the beginning.
	 *
	 * @return the iterator
	 */
	inline auto begin() noexcept
	{
		return m_map.begin();
	}

	/**
	 * Get a const iterator to the beginning.
	 *
	 * @return the iterator
	 */
	inline auto begin() const noexcept
	{
		return m_map.begin();
	}

	/**
	 * Get a const iterator to the beginning.
	 *
	 * @return the iterator
	 */
	inline auto cbegin() const noexcept
	{
		return m_map.cbegin();
	}

	/**
	 * Get a non-const iterator to the end.
	 *
	 * @return the iterator
	 */
	inline auto end() noexcept
	{
		return m_map.end();
	}

	/**
	 * Get a const iterator to the end.
	 *
	 * @return the iterator
	 */
	inline auto end() const noexcept
	{
		return m_map.end();
	}

	/**
	 * Get a const iterator to the end.
	 *
	 * @return the iterator
	 */
	inline auto cend() const noexcept
	{
		return m_map.cend();
	}

	/**
	 * Get the value at the specified key or the defaultValue if key is absent.
	 *
	 * @param name the name
	 * @param defaultValue the value replacement
	 * @return the value or defaultValue
	 */
	template <typename V>
	inline Value valueOr(const std::string &name, V &&defaultValue) const
	{
		auto it = m_map.find(name);

		if (it == m_map.end()) {
			return defaultValue;
		}

		return it->second;
	}

	/**
	 * Get a value from the object.
	 *
	 * @param name the value key
	 * @return the value
	 * @throw std::out_of_range if not found
	 */
	inline const Value &at(const std::string &name) const
	{
		return m_map.at(name);
	}

	/**
	 * Overloaded function.
	 *
	 * @param name the value key
	 * @return the value
	 * @throw std::out_of_range if not found
	 */
	inline Value &at(const std::string &name)
	{
		return m_map.at(name);
	}

	/**
	 * Get a value from the object.
	 *
	 * @param name the value key
	 * @return the value
	 */
	inline Value &operator[](const std::string &name)
	{
		return m_map.at(name);
	}

	/**
	 * Insert a new value.
	 *
	 * @param name the key
	 * @param value the value
	 */
	inline void insert(std::string name, Value value)
	{
		m_map.insert({std::move(name), std::move(value)});
	}

	/**
	 * Remove all the values from the object.
	 */
	inline void clear()
	{
		m_map.clear();
	}

	/**
	 * Get the number of entries in the object.
	 *
	 * @return the size
	 */
	inline unsigned size() const noexcept
	{
		return m_map.size();
	}

	/**
	 * Check if a value exists.
	 *
	 * @param key the key value
	 * @return true if exists
	 */
	inline bool contains(const std::string &key) const noexcept
	{
		return m_map.find(key) != m_map.end();
	}

	/**
	 * Remove a value of the specified key.
	 *
	 * @param key the value key
	 */
	inline void erase(const std::string &key)
	{
		m_map.erase(key);
	}
};

/**
 * @class Buffer
 * @brief Open JSON document from text.
 */
class Buffer {
public:
	std::string text;	//!< The JSON text
};

/**
 * @class File
 * @brief Open JSON document from a file.
 */
class File {
public:
	std::string path;	//!< The path to the file
};

/**
 * @class Document
 * @brief Construct a JSON document from a source
 * @see Buffer
 * @see File
 */
class Document {
private:
	Value m_value;

public:
	/**
	 * Construct a document from a buffer.
	 *
	 * @param buffer the text
	 * @throw Error on errors
	 */
	Document(Buffer buffer);

	/**
	 * Construct a document from a file.
	 *
	 * @param file the file
	 * @throw Error on errors
	 */
	Document(File file);

	/**
	 * Check if the opened document is an object.
	 *
	 * @return true if object
	 */
	inline bool isObject() const noexcept
	{
		return m_value.typeOf() == Type::Object;
	}

	/**
	 * Check if the opened document is an array.
	 *
	 * @return true if array
	 */
	inline bool isArray() const noexcept
	{
		return m_value.typeOf() == Type::Array;
	}

	/**
	 * Get the object.
	 *
	 * @return the object or empty object if not an object
	 */
	inline Object toObject() const noexcept
	{
		if (m_value.typeOf() != Type::Object) {
			return Object{};
		}

		return Object(m_value);
	}

	/**
	 * Get the array.
	 *
	 * @return the array or empty object if not an array
	 */
	inline Array toArray() const noexcept
	{
		if (m_value.typeOf() != Type::Array) {
			return Array{};
		}

		return Array(m_value);
	}
};

/**
 * Escape the input.
 *
 * @param input the input
 * @return the escaped string
 */
std::string escape(std::string input) noexcept;

} // !json

} // !malikania

#endif // !_JSON_H_

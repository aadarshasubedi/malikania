/*
 * Color.h -- color description
 *
 * Copyright (c) 2013-2015 Malikania Authors
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

#ifndef _MALIKANIA_COLOR_H_
#define _MALIKANIA_COLOR_H_

#include <cstdint>

namespace malikania {

/**
 * @class Color
 * @brief Color description
 */
class Color {
private:
	uint8_t m_red{0};
	uint8_t m_green{0};
	uint8_t m_blue{0};
	uint8_t m_alpha{255};

public:
	/**
	 * Default color to black.
	 */
	inline Color() noexcept = default;

	/**
	 * Constructor with all fields.
	 *
	 * @param red the red value
	 * @param green the green value
	 * @param blue the blue value
	 * @param alpha the alpha value
	 */
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept
		: m_red{red}
		, m_green{green}
		, m_blue{blue}
		, m_alpha{alpha}
	{
	}

	/**
	 * Constructor with an hexadecimal value.
	 *
	 * @param hex the color
	 */
	inline Color(uint32_t hex) noexcept
		: m_red((hex >> 16) & 0xff)
		, m_green((hex >> 8) & 0xff)
		, m_blue(hex & 0xff)
		, m_alpha((hex >> 24) & 0xff)
	{
	}

	/**
	 * Construct a color from SVG list. See http://www.december.com/html/spec/colorsvg.html.
	 *
	 * @param name the color name
	 * @throw std::invalid_argument if the color does not exist
	 */
	Color(const std::string &name);

	/**
	 * Get the red value.
	 *
	 * @return the value
	 */
	inline uint8_t red() const noexcept
	{
		return m_red;
	}

	/**
	 * Get the green value.
	 *
	 * @return the value
	 */
	inline uint8_t green() const noexcept
	{
		return m_green;
	}

	/**
	 * Get the blue value.
	 *
	 * @return the value
	 */
	inline uint8_t blue() const noexcept
	{
		return m_blue;
	}

	/**
	 * Get the alpha value.
	 *
	 * @return the value
	 */
	inline uint8_t alpha() const noexcept
	{
		return m_alpha;
	}
};

} // !malikania

#endif // !_MALIKANIA_COLOR_H_

/*
 * Label.h -- GUI label
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

#ifndef _MALIKANIA_LABEL_H_
#define _MALIKANIA_LABEL_H_

#include <malikania/Json.h>

#include <Config.h>

#include "Image.h"
#include "Color.h"
#include "Font.h"
#include "Rectangle.h"

namespace malikania {

class Window;

class Label {
private:
	std::string m_text;
	std::shared_ptr<Font> m_font;
	Rectangle m_frame;
	Color m_textColor;
	Color m_backgroundColor;

public:
	Label(std::string text, std::shared_ptr<Font> font, Rectangle frame, Color textColor = {0, 0, 0, 255}, Color backgroundColor = {255, 255, 255, 0});

	inline const std::string text() const noexcept
	{
		return m_text;
	}

	inline std::string text() noexcept
	{
		return m_text;
	}

	inline const std::shared_ptr<Font> &font() const noexcept
	{
		return m_font;
	}

	inline std::shared_ptr<Font> &font() noexcept
	{
		return m_font;
	}

	int fontSize() const noexcept;

	void draw(Window &window, const Rectangle &rectangle);
};

}// !malikania

#endif // !_MALIKANIA_LABEL_H_

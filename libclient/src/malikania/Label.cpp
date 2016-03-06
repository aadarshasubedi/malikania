/*
 * Label.cpp -- GUI label
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

#include "Label.h"

namespace malikania {

Label::Label(std::string text, std::shared_ptr<Font> font, Rectangle frame, Color textColor, Color backgroundColor)
	: m_text(std::move(text))
	, m_font(std::move(font))
	, m_frame(std::move(frame))
	, m_textColor(std::move(textColor))
	, m_backgroundColor(std::move(backgroundColor))
{
}

int Label::fontSize() const noexcept
{
	// TODO
	return 12;
}

void Label::draw(Window &, const Rectangle &)
{
}

}// !malikania

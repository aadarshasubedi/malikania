/*
 * Sprite.cpp -- image sprite
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

#include "Sprite.h"

namespace malikania {

Sprite::Sprite(Image image, Size cell, Size size, Size space, Size margin) noexcept
	: m_image(std::move(image))
	, m_cell(std::move(cell))
	, m_margin(std::move(margin))
	, m_space(std::move(space))
	, m_size(std::move(size))
{
	assert(m_cell.width() > 0);
	assert(m_cell.height() > 0);

	/* If size is not specified, take from image */
	if (m_size.isNull()) {
		m_size = m_image.size();
	}

	/* Compute number of cells */
	m_rows = (m_size.height() - (margin.height() * 2) + m_space.height()) / (m_cell.height() + m_space.height());
	m_columns = (m_size.width() - (m_margin.width() * 2) + m_space.width()) / (m_cell.width() + m_space.width());
}

void Sprite::draw(Window &window, unsigned cell, const Point &point)
{
	assert(cell < m_rows * m_columns);

	/* Compute index in the grid */
	unsigned hindex = (cell % m_columns);
	unsigned vindex = (cell / m_columns);

	/* Compute the pixel boundaries */
	int x = m_margin.width() + (hindex * m_space.width()) + (hindex * m_cell.width());
	int y = m_margin.height() + (vindex * m_space.height()) + (vindex * m_cell.height());

	Rectangle source(x, y, m_cell.width(), m_cell.height());
	Rectangle target(point.x(), point.y(), m_cell.width(), m_cell.height());

	m_image.draw(window, source, target);
}

} // !malikania

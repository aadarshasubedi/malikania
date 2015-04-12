#include "Size.h"

namespace malikania {

Size::Size(int width, int height)
	: m_width(width)
	, m_height(height)
{
}


int Size::width() const noexcept
{
	return m_width;
}

void Size::setWidth(int width) noexcept
{
	m_width = width;
}

int Size::height() const noexcept
{
	return m_height;
}

void Size::setHeight(int height) noexcept
{
	m_height = height;
}

}// !malikania

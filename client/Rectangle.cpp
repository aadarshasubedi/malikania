#include "Rectangle.h"

namespace malikania {

Rectangle::Rectangle(int x, int y, int width, int height)
{
	m_rectangle.x = x;
	m_rectangle.y = y;
	m_rectangle.h = width;
	m_rectangle.w = height;
}

Rectangle::Rectangle(const Position &position, const Size &size)
{
	m_rectangle.x = position.x();
	m_rectangle.y = position.y();
	m_rectangle.w = size.width();
	m_rectangle.h = size.height();
}

SDL_Rect *Rectangle::get()
{
	return &m_rectangle;
}

Size Rectangle::size() const noexcept
{
	return Size(m_rectangle.w, m_rectangle.h);
}

void Rectangle::setSize(const Size &size) noexcept
{
	m_rectangle.w = size.width();
	m_rectangle.h = size.height();
}

int Rectangle::width() const noexcept
{
	return m_rectangle.w;
}

void Rectangle::setWidth(int width) noexcept
{
	m_rectangle.w = width;
}

int Rectangle::height() const noexcept
{
	return m_rectangle.h;
}

void Rectangle::setHeight(int height) noexcept
{
	m_rectangle.h = height;
}

Position Rectangle::position() const noexcept
{
	return Position(m_rectangle.x, m_rectangle.y);
}

void Rectangle::setPosition(const Position &position) noexcept
{
	m_rectangle.x = position.x();
	m_rectangle.y = position.y();
}

int Rectangle::x() const noexcept
{
	return m_rectangle.x;
}

void Rectangle::setX(int x) noexcept
{
	m_rectangle.x = x;
}

int Rectangle::y() const noexcept
{
	return m_rectangle.y;
}

void Rectangle::setY(int y) noexcept
{
	m_rectangle.y = y;
}

}// !malikania

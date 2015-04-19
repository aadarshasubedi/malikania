#include "Rectangle.h"

namespace malikania {

Rectangle::Rectangle(int x, int y, int width, int height)
	:m_position(Position(x, y)), m_size(Size(width, height))
{
}

Rectangle::Rectangle(Position position, Size size)
	:m_position(std::move(position)), m_size(std::move(size))
{
}

Size Rectangle::size() const noexcept
{
	return m_size;
}

void Rectangle::setSize(Size size) noexcept
{
	m_size = std::move(size);
}

int Rectangle::width() const noexcept
{
	return m_size.width();
}

void Rectangle::setWidth(int width) noexcept
{
	m_size.setWidth(width);
}

int Rectangle::height() const noexcept
{
	return m_size.height();
}

void Rectangle::setHeight(int height) noexcept
{
	m_size.setHeight(height);
}

Position Rectangle::position() const noexcept
{
	return m_position;
}

void Rectangle::setPosition(Position position) noexcept
{
	m_position = std::move(position);
}

int Rectangle::x() const noexcept
{
	return m_position.x();
}

void Rectangle::setX(int x) noexcept
{
	m_position.setX(x);
}

int Rectangle::y() const noexcept
{
	return m_position.y();
}

void Rectangle::setY(int y) noexcept
{
	m_position.setY(y);
}

}// !malikania

#include "Position.h"

namespace malikania {

Position::Position(int x, int y)
	: m_x(x)
	, m_y(y)
{
}


int Position::x() const noexcept
{
	return m_x;
}

void Position::setX(int x) noexcept
{
	m_x = x;
}

int Position::y() const noexcept
{
	return m_y;
}

void Position::setY(int y) noexcept
{
	m_y = y;
}

}// !malikania

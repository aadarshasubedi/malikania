#ifndef _MALIKANIA_RECTANGLE_H_
#define _MALIKANIA_RECTANGLE_H_

namespace malikania {

class Rectangle {
private:
	int m_x;
	int m_y;
	unsigned m_width;
	unsigned m_height;

public:
	inline Rectangle(int x = 0, int y = 0, unsigned width = 0, unsigned height = 0) noexcept
		: m_x(x)
		, m_y(y)
		, m_width(width)
		, m_height(height)
	{
	}

	inline int x() const noexcept
	{
		return m_x;
	}

	inline int y() const noexcept
	{
		return m_y;
	}

	inline unsigned width() const noexcept
	{
		return m_width;
	}

	inline unsigned height() const noexcept
	{
		return m_height;
	}

	inline bool isNull() const noexcept
	{
		return m_width == 0 && m_height == 0;
	}
};

inline bool operator==(const Rectangle &r1, const Rectangle &r2) noexcept
{
	return r1.x() == r2.x() && r1.y() == r2.y() && r1.width() == r2.width() && r1.height() == r2.height();
}

inline bool operator!=(const Rectangle &r1, const Rectangle &r2) noexcept
{
	return !(r1 == r2);
}

} // !malikania

#endif // !_MALIKANIA_RECTANGLE_H_

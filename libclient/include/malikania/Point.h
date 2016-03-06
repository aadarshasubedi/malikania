#ifndef _MALIKANIA_POINT_H_
#define _MALIKANIA_POINT_H_

namespace malikania {

class Point {
private:
	int m_x;
	int m_y;

public:
	inline Point(int x = 0, int y = 0) noexcept
		: m_x(x)
		, m_y(y)
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
};

inline bool operator==(const Point &p1, const Point &p2) noexcept
{
	return p1.x() == p2.x() && p1.x() == p2.x();
}

inline bool operator!=(const Point &p1, const Point &p2) noexcept
{
	return !(p1 == p2);
}

} // !malikania

#endif // !_MALIKANIA_POINT_H_

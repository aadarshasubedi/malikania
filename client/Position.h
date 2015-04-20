#ifndef POSITION_H
#define POSITION_H

namespace malikania {

class Position
{
private:
	int m_x;
	int m_y;

public:
	Position(int x = 0, int y = 0);
	int x() const noexcept;
	void setX(int x) noexcept;
	int y() const noexcept;
	void setY(int y) noexcept;
};

}// !malikania

#endif // POSITION_H

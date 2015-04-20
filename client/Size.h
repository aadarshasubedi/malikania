#ifndef SIZE_H
#define SIZE_H

namespace malikania {

class Size
{
private:
	int m_width;
	int m_height;

public:
	Size(int width = 0, int height = 0);
	int width() const noexcept;
	void setWidth(int width) noexcept;
	int height() const noexcept;
	void setHeight(int height) noexcept;
};

}// !malikania

#endif // SIZE_H

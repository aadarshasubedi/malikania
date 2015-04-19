#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <utility>
#include <SDL.h>
#include "Size.h"
#include "Position.h"

namespace malikania {

class Rectangle {
private:
	Position m_position;
	Size m_size;

public:
	Rectangle(int x, int y, int width, int height);
	Rectangle(Position position, Size size);
	SDL_Rect *get();
	Size size() const noexcept;
	void setSize(Size size) noexcept;
	int width() const noexcept;
	void setWidth(int width) noexcept;
	int height() const noexcept;
	void setHeight(int height) noexcept;
	Position position() const noexcept;
	void setPosition(Position position) noexcept;
	int x() const noexcept;
	void setX(int x) noexcept;
	int y() const noexcept;
	void setY(int y) noexcept;
};

}// !malikania

#endif // RECTANGLE_H

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL.h>
#include "Size.h"
#include "Position.h"

namespace malikania {

class Rectangle {
private:
	SDL_Rect m_rectangle;

public:
	Rectangle(int x, int y, int width, int height);
	Rectangle(const Position &position, const Size &size);
	SDL_Rect *get();
	Size size() const noexcept;
	void setSize(const Size &size) noexcept;
	int width() const noexcept;
	void setWidth(int width) noexcept;
	int height() const noexcept;
	void setHeight(int height) noexcept;
	Position position() const noexcept;
	void setPosition(const Position &position) noexcept;
	int x() const noexcept;
	void setX(int x) noexcept;
	int y() const noexcept;
	void setY(int y) noexcept;
};

}// !malikania

#endif // RECTANGLE_H

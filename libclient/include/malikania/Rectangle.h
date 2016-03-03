#ifndef _MALIKANIA_RECTANGLE_H_
#define _MALIKANIA_RECTANGLE_H_

#include <utility>
#include <SDL.h>

namespace malikania {

class Rectangle {
public:
	int x{0};
	int y{0};
	int width{0};
	int height{0};
};

}// !malikania

#endif // !_MALIKANIA_RECTANGLE_H_

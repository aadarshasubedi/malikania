#ifndef SPRITESDL_H
#define SPRITESDL_H

#include <malikania/Json.h>

#include "Rectangle.h"
#include "Size.h"

namespace malikania {

class Window;
class Sprite;

class SpriteSdl {
public:
	void render(Sprite &sprite, Window &window, int leftPosition, int topPosition, const Rectangle &rectangle);
	Size size(Sprite &sprite);
};

/*
 * "Export" the backend.
 */
using BackendSprite = SpriteSdl;

}// !malikania

#endif // SPRITESDL_H

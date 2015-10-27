#ifndef _MALIKANIA_SPRITESDL_H_
#define _MALIKANIA_SPRITESDL_H_

#include <malikania/Json.h>

#include <malikania/Rectangle.h>
#include <malikania/Size.h>

namespace malikania {

class Window;
class Sprite;

/**
 * @brief The SpriteSdl class
 */
class SpriteSdl {
public:
	/**
	 * @brief SpriteSdl::render
	 * @param sprite
	 * @param window
	 * @param leftPosition
	 * @param topPosition
	 * @param rectangle - Destination rectangle
	 */
	void render(Sprite &sprite, Window &window, int leftPosition, int topPosition, const Rectangle &rectangle);
	Size size(Sprite &sprite);
};

/*
 * "Export" the backend.
 */
using BackendSprite = SpriteSdl;

}// !malikania

#endif // !_MALIKANIA_SPRITESDL_H_

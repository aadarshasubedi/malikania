#ifndef SPRITE_H
#define SPRITE_H

#include <malikania/Json.h>

#include "Image.h"

namespace malikania {

class Window;

class Sprite {
private:
	Image m_image;
	std::string m_name;
	Size m_cell;
	Size m_size;
	Size m_space;
	Size m_margin;
	static void checkJSONFormat(const JsonObject& json);

public:
	Sprite(Image image, std::string alias, Size cell, Size size, Size space = {0, 0}, Size margin = {0, 0});
	static Sprite fromJson(Window &window, const JsonObject &jsonSprite);
	void draw(Window &window, int index, const Rectangle &rectangle);
};

}// !malikania

#endif // SPRITE_H

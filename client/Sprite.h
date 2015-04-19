#ifndef SPRITE_H
#define SPRITE_H

#include "Image.h"
#include <malikania/Json.h>

namespace malikania {

class Sprite
{
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
	static Sprite fromJson(const JsonObject& jsonSprite);
	void draw(int index, const Rectangle &rectangle);
};

}// !malikania

#endif // SPRITE_H

#ifndef _MALIKANIA_ANIMATION_H_
#define _MALIKANIA_ANIMATION_H_

#include <vector>
#include <map>
#include <string>

#include <malikania/Json.h>

#include <Config.h>

#include "Sprite.h"

namespace malikania {

class Window;

class Animation {
private:
	Sprite m_sprite;
	std::string m_name;
	std::vector<std::map<std::string, int>> m_frames;
	int m_spendedTime = 0;
	int m_currentFrame = 0;
	static void checkJSONFormat(const JsonObject& json);

public:
	Animation(Sprite image, std::string alias, std::vector<std::map<std::string, int>> frames);

	inline const Sprite &sprite() const noexcept
	{
		return m_sprite;
	}

	inline Sprite &sprite() noexcept
	{
		return m_sprite;
	}

	static Animation fromJson(Window &window, const JsonObject &jsonAnimation);

	void draw(Window &window, const Rectangle &rectangle);
};

}// !malikania

#endif // !_MALIKANIA_ANIMATION_H_

#ifndef _MALIKANIA_ANIMATOR_H_
#define _MALIKANIA_ANIMATOR_H_

#include <vector>
#include <map>
#include <string>
#include <malikania/ElapsedTimer.h>

#include <malikania/Json.h>

#include <Config.h>

#include "Animation.h"

namespace malikania {

class Window;

class Animator {
private:
	std::shared_ptr<Animation> m_animation;
	ElapsedTimer m_timer;
	int m_currentFrame = 0;

public:
	inline Animator(std::shared_ptr<Animation> animation)
		: m_animation(std::move(animation))
	{
	}
	
	inline const std::shared_ptr<Animation> &animation() const noexcept
	{
		return m_animation;
	}

	inline std::shared_ptr<Animation> &animation() noexcept
	{
		return m_animation;
	}

	void draw(Window &window, const Rectangle &rectangle);
};

}// !malikania

#endif // !_MALIKANIA_ANIMATOR_H_

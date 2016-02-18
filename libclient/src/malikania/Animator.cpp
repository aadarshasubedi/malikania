#include "Animator.h"
#include "Window.h"

namespace malikania {

void Animator::draw(Window &window, const Rectangle &rectangle)
{
	std::map<std::string, int> frame = m_animation->frames().at(m_currentFrame);

	m_animation->sprite().draw(window, frame["cell"], rectangle);
	if (m_timer.elapsed() > frame["delay"]) {
		m_currentFrame++;
		m_timer.reset();
		// loop the animation
		if (m_currentFrame > (m_animation->frames().size() - 1)) {
			m_currentFrame = 0;
		}
	}
}

}// !malikania

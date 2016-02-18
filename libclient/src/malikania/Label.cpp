#include <memory>

#include "Label.h"
#include "Window.h"

namespace malikania {

Label::Label(std::string text, std::shared_ptr<Font> font, Rectangle frame, Color textColor, Color backgroundColor)
	: m_text(std::move(text))
	, m_font(std::move(font))
	, m_frame(std::move(frame))
	, m_textColor(std::move(textColor))
	, m_backgroundColor(std::move(backgroundColor))
{
}

const int Label::fontSize() const
{
	// TODO
	return 12;
}

void Label::draw(Window &window, const Rectangle &rectangle)
{
	m_backend.render(*this, window, rectangle);
}

}// !malikania

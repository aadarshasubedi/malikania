#ifndef _MALIKANIA_LABEL_H_
#define _MALIKANIA_LABEL_H_

#include <malikania/Json.h>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/LabelSdl.h"
#endif

#include "Image.h"
#include "Color.h"
#include "Font.h"

namespace malikania {

class Window;

class Label {
private:
	BackendLabel m_backend;
	std::string m_text;
	std::shared_ptr<Font> m_font;
	Rectangle m_frame;
	Color m_textColor;
	Color m_backgroundColor;

public:
	Label(std::string text, std::shared_ptr<Font> font, Rectangle frame, Color textColor = {0, 0, 0, 255}, Color backgroundColor = {255, 255, 255, 0});

	inline const std::string text() const noexcept
	{
		return m_text;
	}

	inline std::string text() noexcept
	{
		return m_text;
	}

	inline const std::shared_ptr<Font> &font() const noexcept
	{
		return m_font;
	}

	inline std::shared_ptr<Font> &font() noexcept
	{
		return m_font;
	}

	int fontSize() const noexcept;

	void draw(Window &window, const Rectangle &rectangle);

	inline BackendLabel &backend() noexcept
	{
		return m_backend;
	}
};

}// !malikania

#endif // !_MALIKANIA_LABEL_H_

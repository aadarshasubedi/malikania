#ifndef _MALIKANIA_SPRITE_H_
#define _MALIKANIA_SPRITE_H_

#include <malikania/Json.h>

#include <Config.h>

#if defined(WITH_BACKEND_SDL)
#  include "backend/sdl/SpriteSdl.h"
#endif

#include "Image.h"

namespace malikania {

class Window;

class Sprite {
private:
	BackendSprite m_backend;
	Image m_image;
	std::string m_name;
	Size m_cell;
	Size m_size;
	Size m_space;
	Size m_margin;

public:
	Sprite(Image image, std::string alias, Size cell, Size size, Size space = {0, 0}, Size margin = {0, 0});

	inline const Image &image() const noexcept
	{
		return m_image;
	}

	inline Image &image() noexcept
	{
		return m_image;
	}

	inline Size &cell() noexcept
	{
		return m_cell;
	}

	inline const Size &cell() const noexcept
	{
		return m_cell;
	}

	inline Size &space() noexcept
	{
		return m_space;
	}

	inline const Size &space() const noexcept
	{
		return m_space;
	}

	inline Size &size()
	{
		m_size = m_size.height > 0 && m_size.width > 0 ? m_size : m_backend.size(*this);

		return m_size;
	}

	inline void setSize(Size size)
	{
		m_size = std::move(size);
	}

	inline Size margin() noexcept
	{
		return m_margin;
	}

	inline BackendSprite &backend() noexcept
	{
		return m_backend;
	}

	void draw(Window &window, int index, const Rectangle &rectangle);
};

}// !malikania

#endif // !_MALIKANIA_SPRITE_H_

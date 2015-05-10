#include <stdexcept>

#include "Image.h"
#include "Window.h"

#include <SDL_image.h>

namespace malikania {

const Size &Image::size() const noexcept
{
	return m_size;
}

void Image::setSize(Size size) noexcept
{
	m_size = std::move(size);
}

}// !malikania

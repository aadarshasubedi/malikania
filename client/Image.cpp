#include <stdexcept>

#include "Image.h"
#include "Window.h"

#include <SDL_image.h>

namespace malikania {

Image::Image(std::string imagePath, Size size)
	: m_texture(nullptr, nullptr)
	, m_size(std::move(size))
{
	// Create Texture
	m_texture = TextureHandle(
		IMG_LoadTexture(Window::renderer().get(), imagePath.c_str()),
		SDL_DestroyTexture
	);

	if (m_texture == NULL) {
		std::string error = "Couldn't create a texture: " + std::string(SDL_GetError());
		throw std::runtime_error(error);
	}
}

TextureHandle& Image::getTexture()
{
	return m_texture;
}

int Image::width() const noexcept
{
	return m_size.width();
}

void Image::setWidth(int width) noexcept
{
	m_size.setWidth(width);
}

int Image::height() const noexcept
{
	return m_size.height();
}

void Image::setHeight(int height) noexcept
{
	m_size.setHeight(height);
}

const Size &Image::size() const noexcept
{
	return m_size;
}

void Image::setSize(Size size) noexcept
{
	m_size = std::move(size);
}

}// !malikania

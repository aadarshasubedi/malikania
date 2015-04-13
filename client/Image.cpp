#include <stdexcept>

#include "Image.h"

#include <SDL_image.h>

namespace malikania {

Image::Image(std::string imagePath, const RendererHandle &renderer, const malikania::Rectangle &rectangle)
	: m_texture(nullptr, nullptr)
	, m_position(rectangle.x(), rectangle.y())
	, m_rectangle(0, 0, rectangle.width(), rectangle.height())
{
	// Create Texture
	m_texture = TextureHandle(
		IMG_LoadTexture(renderer.get(), imagePath.c_str()),
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

int Image::x() const noexcept
{
	return m_position.x();
}

int Image::y() const noexcept
{
	return m_position.y();
}

void Image::setX(int x) noexcept
{
	m_position.setX(x);
}

void Image::setY(int y) noexcept
{
	m_position.setY(y);
}

int Image::width() const noexcept
{
	return m_rectangle.width();
}

void Image::setWidth(int width) noexcept
{
	m_rectangle.setWidth(width);
}

int Image::height() const noexcept
{
	return m_rectangle.height();
}

void Image::setHeight(int height) noexcept
{
	m_rectangle.setHeight(height);
}

Position Image::getPosition() const noexcept
{
	return m_position;
}

void Image::setPosition(const Position &position) noexcept
{
	m_position = position;
}

void Image::setPosition(int x, int y) noexcept
{
	m_position.setX(x);
	m_position.setY(y);
}

Rectangle &Image::getRectangle() noexcept
{
	return m_rectangle;
}

}// !malikania

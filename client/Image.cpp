#include "Image.h"
#include <SDL_image.h>

Image::Image(std::string imagePath, const RendererHandle &renderer, int x, int y)
	: m_texture(nullptr, nullptr), m_x(x), m_y(y)
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

int Image::getX() const noexcept
{
	return m_x;
}

int Image::getY() const noexcept
{
	return m_y;
}

void Image::setX(int x) noexcept
{
	m_x = x;
}

void Image::setY(int y) noexcept
{
	m_y = y;
}

void Image::setPosition(int x, int y) noexcept
{
	setX(x);
	setY(y);
}

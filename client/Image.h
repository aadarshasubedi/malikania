#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <string>

#include <SDL.h>

#include "Rectangle.h"
#include "Position.h"

namespace malikania {

using TextureHandle = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>;
using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;

class Image
{
protected:
	TextureHandle m_texture;
	Size m_size;
	Position m_position;

public:
	Image(std::string imagePath, Size size = {0, 0});
	TextureHandle &getTexture();
	int width() const noexcept;
	void setWidth(int width) noexcept;
	int height() const noexcept;
	void setHeight(int height) noexcept;
	const Size &size() const noexcept;
	void setSize(Size size) noexcept;
};

}// !malikania

#endif // IMAGE_H

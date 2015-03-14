#ifndef IMAGE_H
#define IMAGE_H

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
	Rectangle m_rectangle;
	Position m_position;
public:
	Image(std::string imagePath, const RendererHandle &renderer, const Rectangle &rectangle);
	TextureHandle &getTexture();
	int x() const noexcept;
	void setX(int x) noexcept;
	int y() const noexcept;
	void setY(int y) noexcept;
	int width() const noexcept;
	void setWidth(int width) noexcept;
	int height() const noexcept;
	void setHeight(int height) noexcept;
	Position getPosition() const noexcept;
	void setPosition(const Position &position) noexcept;
	void setPosition(int x, int y) noexcept;
	virtual Rectangle &getRectangle() noexcept;
};

}// !malikania

#endif // IMAGE_H

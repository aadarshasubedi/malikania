#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <SDL.h>

using TextureHandle = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>;
using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;
using RectangleHandle = std::unique_ptr<SDL_Rect>;

class Image
{
protected:
	TextureHandle m_texture;
	RectangleHandle m_rectangle;
	int m_x;
	int m_y;
public:
	Image(std::string imagePath, const RendererHandle &renderer, int width, int height, int x = 0, int y = 0);
	TextureHandle &getTexture();
	int getX() const noexcept;
	int getY() const noexcept;
	void setX(int x) noexcept;
	void setY(int y) noexcept;
	void setPosition(int x, int y) noexcept;
	int getWidth() const;
	int getHeight() const;
	virtual RectangleHandle &getRectangle();
};

#endif // IMAGE_H

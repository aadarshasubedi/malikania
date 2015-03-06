#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <SDL.h>

using TextureHandle = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>;
using RendererHandle = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>;

class Image
{
private:
	TextureHandle m_texture;
	int m_x;
	int m_y;
public:
	Image(std::string imagePath, const RendererHandle &renderer, int x = 0, int y = 0);
	TextureHandle &getTexture();
	int getX() const noexcept;
	int getY() const noexcept;
	void setX(int x) noexcept;
	void setY(int y) noexcept;
	void setPosition(int x, int y) noexcept;
};

#endif // IMAGE_H

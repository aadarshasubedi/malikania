#ifndef _MALIKANIA_IMAGE_SDL_H_
#define _MALIKANIA_IMAGE_SDL_H_

#include <memory>

#include <SDL.h>

namespace malikania {

class Image;
class Point;
class Window;

using TextureHandle = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>;

class ImageSdl {
private:
	TextureHandle m_texture;

public:
	ImageSdl(Image &image, Window &window, const std::string &data);

	inline SDL_Texture *texture() noexcept
	{
		return m_texture.get();
	}

	void draw(Window &window, const Point &position);

};

/*
 * "Export" the backend.
 */
using BackendImage = ImageSdl;

} // !malikania

#endif // !_MALIKANIA_IMAGE_SDL_H_

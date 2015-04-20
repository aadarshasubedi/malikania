#ifndef _MALIKANIA_IMAGE_SDL_H_
#define _MALIKANIA_IMAGE_SDL_H_

#include <SDL.h>

namespace malikania {

using TextureHandle = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>;

class ImageSdl {
private:
	TextureHandle m_texture;

public:
	ImageSdl(const std::string &path);

};

/*
 * "Export" the backend.
 */
using BackendImage = ImageSdl;

} // !malikania

#endif // !_MALIKANIA_IMAGE_SDL_H_

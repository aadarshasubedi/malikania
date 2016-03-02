#include <SDL_image.h>

#include <Window.h>

#include <malikania/backend/sdl/ImageSdl.h>

using namespace std::string_literals;

namespace malikania {

ImageSdl::ImageSdl(Image &, Window &window, const std::string &data)
	: m_texture(nullptr, nullptr)
{
	auto rw = SDL_RWFromMem(const_cast<char *>(data.c_str()), data.length());

	if (rw == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	m_texture = TextureHandle(IMG_LoadTexture_RW(window.backend().renderer(), rw, true), SDL_DestroyTexture);

	if (m_texture == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}
}

void ImageSdl::draw(Window &, const Point &)
{
}

} // !malikania

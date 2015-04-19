#include "ImageSdl.h"

using namespace std::string_literals;

namespace malikania {

ImageSdl::ImageSdl(Image &image, Window &window, const std::string &path)
	: m_texture(nullptr, nullptr)
	, m_size(std::move(size))
{
	// Create Texture
	m_texture = TextureHandle(
		IMG_LoadTexture(window.backend().renderer(), path.c_str()),
		SDL_DestroyTexture
	);

	if (m_texture == nullptr) {
		throw std::runtime_error("failed to load texture: "s + std::string(SDL_GetError()));
	}
}

} // !malikania

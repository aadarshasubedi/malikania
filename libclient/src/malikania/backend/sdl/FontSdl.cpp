#include <Window.h>

#include <malikania/backend/sdl/FontSdl.h>

using namespace std::string_literals;

namespace malikania {

FontSdl::FontSdl(std::string path, int size)
	: m_path(std::move(path))
	, m_size(size)
	, m_font(nullptr, nullptr)
{
	m_font = FontHandle(
		TTF_OpenFont(m_path.c_str(), m_size),
		TTF_CloseFont
	);
	if (m_font == NULL) {
		throw std::runtime_error(std::string("Unable to load font:") + TTF_GetError());
	}
}

} // !malikania

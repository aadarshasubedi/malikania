#ifndef _CLIENT_RESOURCES_LOADER_H_
#define _CLIENT_RESOURCES_LOADER_H_

#include <malikania/ResourcesLoader.h>

namespace malikania {

class Image;
class Sprite;
class Window;

class ClientResourcesLoader : public ResourcesLoader {
private:
	Window &m_window;

public:
	inline ClientResourcesLoader(Window &window, ResourcesLocator &locator)
		: ResourcesLoader(locator)
		, m_window(window)
	{
	}

	virtual Image loadImage(const std::string &id);
	virtual Sprite loadSprite(const std::string &id);
};

} // !malikania

#endif // !_CLIENT_RESOURCES_LOADER_H_

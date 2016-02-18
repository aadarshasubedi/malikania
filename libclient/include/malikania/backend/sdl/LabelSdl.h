#ifndef _MALIKANIA_LABEL_SDL_H_
#define _MALIKANIA_LABEL_SDL_H_

#include <malikania/Json.h>

#include <malikania/Rectangle.h>
#include <malikania/Size.h>

namespace malikania {

class Window;
class Label;

/**
 * @brief The LabelSdl class
 */
class LabelSdl {
public:
	/**
	 * @brief LabelSdl::render
	 * @param sprite
	 * @param window
	 * @param leftPosition
	 * @param topPosition
	 * @param rectangle - Destination rectangle
	 */
	void render(Label &label, Window &window, const Rectangle &rectangle);
};

/*
 * "Export" the backend.
 */
using BackendLabel = LabelSdl;

}// !malikania

#endif // !_MALIKANIA_LABEL_SDL_H_

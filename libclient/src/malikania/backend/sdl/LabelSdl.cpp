#include <malikania/backend/sdl/LabelSdl.h>

#include "Label.h"
#include "Window.h"

namespace malikania {

void LabelSdl::render(Label &label, Window &window, const Rectangle &rectangle)
{
	// TODO draw borders and other things
	window.drawText(label.text(), *label.font(), Point{rectangle.x, rectangle.y});
}

} // !malikania

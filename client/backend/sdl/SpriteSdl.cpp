#include "SpriteSdl.h"

#include "Sprite.h"
#include "Window.h"

namespace malikania {

Size SpriteSdl::size(Sprite &sprite)
{
	int width;
	int height;
	SDL_QueryTexture(sprite.image().backend().texture(), nullptr, nullptr, &width, &height);
	return Size(width, height);
}

void SpriteSdl::render(Sprite &sprite, Window &window, int leftPosition, int topPosition, const Rectangle &rectangle)
{
	// Source rectangle (a particular cell of the sprite)
	SDL_Rect sourceRectangle;
	sourceRectangle.x = leftPosition;
	sourceRectangle.y = topPosition;
	sourceRectangle.w = sprite.cell().width();
	sourceRectangle.h = sprite.cell().height();

	// Destination rectangle (define the rectangle where the texture will be displayed)
	SDL_Rect destinationRectangle;
	destinationRectangle.x = rectangle.x();
	destinationRectangle.y = rectangle.y();
	destinationRectangle.w = rectangle.width();
	destinationRectangle.h = rectangle.height();

	SDL_RenderCopy(window.backend().renderer(), sprite.image().backend().texture(), &sourceRectangle, &destinationRectangle);
}

} // !malikania

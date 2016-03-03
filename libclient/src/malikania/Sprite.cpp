#include "Sprite.h"
#include "Window.h"

namespace malikania {

Sprite::Sprite(Image image, Size cell, Size size, Size space, Size margin) noexcept
	: m_image(std::move(image))
	, m_cell(std::move(cell))
	, m_size(std::move(size))
	, m_space(std::move(space))
	, m_margin(std::move(margin))
{
}

void Sprite::draw(Window &, int, const Rectangle &)
{
#if 0
	std::string prependErrorMessage = "Couldn't draw image from Sprite " + m_name + ": ";

	// Check if number of image by line is correct
	if ((m_size.width - m_margin.width) % (m_cell.width + m_space.width) != 0) {
		throw std::runtime_error(prependErrorMessage + "cell, margin and space don't fit the global sprite width correctly");
	}

	int numberOfImageByLine = (m_size.width - m_margin.width) / (m_cell.width + m_space.width);
	int verticalIndex = index / numberOfImageByLine;
	int horizontalIndex = index % numberOfImageByLine;

	// First, take margin width and height
	int leftPosition = m_margin.width;
	int topPosition = m_margin.height;

	// Second, take cell width and height
	leftPosition += (m_cell.height * horizontalIndex);
	topPosition += (m_cell.width * verticalIndex);

	// Third, take space (padding) between cells width and height
	leftPosition += (m_space.height * horizontalIndex);
	topPosition += (m_space.width * verticalIndex);

	m_backend.render(*this, window, leftPosition, topPosition, rectangle);
#endif
}

}// !malikania

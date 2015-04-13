#include "Animation.h"
#include <iostream>

namespace malikania {

Animation::Animation(std::string imagePath, const RendererHandle &renderer, const Rectangle &rectangle, Size cellSize, std::string defaultState)
	:Image(imagePath, renderer, rectangle)
	, m_cellSize(cellSize.width(), cellSize.height())
	, m_currentState(defaultState)
{
	m_rectangle.setSize(m_cellSize);
	// set default position to x = 0, y = 0
	m_cellMap[defaultState] = Position(0, 0);
}

void Animation::setState(std::string state)
{
	m_currentState = state;
}

Rectangle &Animation::getRectangle() noexcept
{
	if (m_cellMap.find(m_currentState) != m_cellMap.end()) {
		const Position &cell = m_cellMap[m_currentState];
		m_rectangle.setX(cell.x());
		m_rectangle.setY(cell.y());
	} else {
		throw std::runtime_error("Couldn't find \"" + m_currentState + "\" in animation states");
	}

	return m_rectangle;
}

void Animation::setCellMap(std::map<std::string, Position> cellMap)
{
	m_cellMap = cellMap;
}

}// !malikania

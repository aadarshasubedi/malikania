#include "Animation.h"
#include <iostream>

Animation::Animation(std::string imagePath, const RendererHandle &renderer, int width, int height, int cellWidth, int cellHeight, std::string defaultState, int x, int y)
	:Image(imagePath, renderer, width, height, x, y), m_cellWidth(cellWidth), m_cellHeight(cellHeight), m_currentState(defaultState)
{
	m_rectangle->w = m_cellWidth;
	m_rectangle->h = m_cellHeight;
	// set default position to x = 0, y = 0
	m_cellMap[defaultState] = std::make_tuple(0, 0);
}

void Animation::setState(std::string state)
{
	m_currentState = state;
}

RectangleHandle &Animation::getRectangle()
{
	if (m_cellMap.find(m_currentState) != m_cellMap.end()) {
		std::tuple<int, int> cell = m_cellMap[m_currentState];
		m_rectangle->x = std::get<0>(cell);
		m_rectangle->y = std::get<1>(cell);
	} else {
		throw std::runtime_error("Couldn't find \"" + m_currentState + "\" in animation states");
	}

	return m_rectangle;
}

void Animation::setCellMap(std::map<std::string, std::tuple<int, int>> cellMap)
{
	m_cellMap = cellMap;
}

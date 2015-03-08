#ifndef ANIMATION_H
#define ANIMATION_H

#include <tuple>
#include <map>
#include "Image.h"

class Animation : public Image
{
private:
	int m_cellWidth;
	int m_cellHeight;
	std::map<std::string, std::tuple<int, int>> m_cellMap;
	std::string m_currentState;

public:
	Animation(std::string imagePath, const RendererHandle &renderer, int width, int height,
		int cellWidth, int cellHeight, std::string defaultState = "default", int x = 0, int y = 0);
	virtual RectangleHandle &getRectangle() override;
	void setCellMap(std::map<std::string, std::tuple<int, int>> cellMap);
	void setState(std::string state);
};

#endif // ANIMATION_H

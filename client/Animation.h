#ifndef ANIMATION_H
#define ANIMATION_H

#include <map>
#include "Image.h"
#include "Size.h"

namespace malikania {

class Animation : public Image
{
private:
	Size m_cellSize;
	std::map<std::string, Position> m_cellMap;
	std::string m_currentState;

public:
	Animation(std::string imagePath, const RendererHandle &renderer, const Rectangle &rectangle,
		Size cellSize, std::string defaultState = "default");
	virtual Rectangle &getRectangle() noexcept override;
	void setCellMap(std::map<std::string, Position> cellMap);
	void setState(std::string state);
};

}// !malikania

#endif // ANIMATION_H

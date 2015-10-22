#include "LightShapeMaker.h"

LightShapeMaker::LightShapeMaker()
{
}


LightShapeMaker::~LightShapeMaker()
{
}

int LightShapeMaker::addHexToList(std::map<int, std::shared_ptr<ltbl::LightShape>>& list, sf::Vector2f mouse)
{
	int Index = list.size() + 1;
	
	list[Index] = std::make_shared<ltbl::LightShape>();

	list[Index]->_shape.setPointCount(6);

	list[Index]->_shape.setPoint(0, sf::Vector2f(25, -43));
	list[Index]->_shape.setPoint(1, sf::Vector2f(-25, -43));
	list[Index]->_shape.setPoint(2, sf::Vector2f(-50, 0));
	list[Index]->_shape.setPoint(3, sf::Vector2f(-25, 43));
	list[Index]->_shape.setPoint(4, sf::Vector2f(25, 43));
	list[Index]->_shape.setPoint(5, sf::Vector2f(50, 0));

	list[Index]->_shape.setPosition(mouse);

	list[Index]->_renderLightOverShape = true;

	return Index;
}

void LightShapeMaker::begin(std::map<int, std::shared_ptr<ltbl::LightShape>>& list, int pointCnt)
{
	if (_active)
		return;

	_lsIndex = list.size() + 1;
	_active = true;
	_pntIndex = 0;
	_ls = std::make_shared<ltbl::LightShape>();
	_ls->_shape.setPointCount(pointCnt);
	
	list[_lsIndex] = _ls;
}

bool LightShapeMaker::isActive()
{
	return _active;
}

void LightShapeMaker::setPosition(sf::Vector2f pos)
{
	if(_active)
		_ls->_shape.setPosition(pos);
}

void LightShapeMaker::addPoint(sf::Vector2f pos)
{
	if (_active && _pntIndex < _ls->_shape.getPointCount())
		_ls->_shape.setPoint(_pntIndex++, pos-_ls->_shape.getPosition());
}

int LightShapeMaker::finish()
{
	if (!_active)
		return -1;

	if (_pntIndex < _ls->_shape.getPointCount() - 1)
		_ls->_shape.setPointCount(_pntIndex+1);

	_active = false;
	return _lsIndex;
}

int LightShapeMaker::addShapeToList(std::map<int, std::shared_ptr<ltbl::LightShape>>& list, sf::Vector2f mouse, std::vector<sf::Vector2f>& pts)
{
	int Index = list.size() + 1;

	list[Index] = std::make_shared<ltbl::LightShape>();

	list[Index]->_shape.setPointCount(pts.size());

	for (int i = 0; i < pts.size(); i++)
	{
		list[Index]->_shape.setPoint(i, pts.at(i));
	}

	list[Index]->_shape.setPosition(mouse);

	list[Index]->_renderLightOverShape = true;

	return Index;
}

#pragma once

#include <ltbl\lighting\LightShape.h>

#include <SFML\Window\Mouse.hpp>

class LightShapeMaker
{
public:
	LightShapeMaker();
	~LightShapeMaker();

	size_t addHexToList(std::vector<std::shared_ptr<ltbl::LightShape>>&, sf::Vector2f);
	size_t addShapeToList(std::vector<std::shared_ptr<ltbl::LightShape>>&, sf::Vector2f, std::vector<sf::Vector2f>&);

	void begin(std::vector<std::shared_ptr<ltbl::LightShape>>&, int);
	bool isActive();
	void setPosition(sf::Vector2f);
	void addPoint(sf::Vector2f);
	size_t finish();

private:
	bool _active = false;
	size_t _lsIndex;
	int _pntIndex;
	std::shared_ptr<ltbl::LightShape> _ls;
};


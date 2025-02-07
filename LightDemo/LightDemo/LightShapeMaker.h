#pragma once

#include <ltbl\lighting\LightShape.h>

#include <SFML\Window\Mouse.hpp>

class LightShapeMaker
{
public:
	LightShapeMaker();
	~LightShapeMaker();

	int addHexToList(std::vector<std::shared_ptr<ltbl::LightShape>>&, sf::Vector2f);
	int addShapeToList(std::vector<std::shared_ptr<ltbl::LightShape>>&, sf::Vector2f, std::vector<sf::Vector2f>&);

	void begin(std::vector<std::shared_ptr<ltbl::LightShape>>&, int);
	bool isActive();
	void setPosition(sf::Vector2f);
	void addPoint(sf::Vector2f);
	int finish();

private:
	bool _active = false;
	int _lsIndex;
	int _pntIndex;
	std::shared_ptr<ltbl::LightShape> _ls;
};


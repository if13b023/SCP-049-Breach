#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Map: public GameObject
{
public:
	Map();
	~Map();

	sf::Texture diffuseMap;
	sf::Texture collisionMap;

	sf::Sprite getDiffuseSprite();

	int LoadFromFile(char*, char*);

private:
	sf::Sprite diffuseSprite;
	sf::Sprite collisionSprite;
};


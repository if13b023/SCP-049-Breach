#pragma once

#include <SFML/Graphics.hpp>

class Map
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


#pragma once
#include <string>
#include <SFML/Graphics.hpp>
class Map
{
public:
	Map();
	~Map();

	sf::Texture diffuseMap;
	sf::Texture collisionMap;

	sf::Sprite diffuseSprite;
	sf::Sprite collisionSprite;

	int LoadFromFile(std::string);
};


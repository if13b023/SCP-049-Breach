#pragma once
#include "Zombie.h"
#include "SFML\Graphics.hpp"
#include <iostream>

class Zombies
{
public:
	Zombies();
	~Zombies();

	size_t addZombie();
	Zombie getZombie(int);

	size_t count();

	bool setTexture(const char*);
	bool setTexture(sf::Texture);

private:
	std::vector<Zombie> m_list;
	sf::Texture m_tex;
	sf::Sprite m_sprite;
};


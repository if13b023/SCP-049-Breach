#pragma once
#include "Zombie.h"
#include "SFML\Graphics.hpp"
#include <iostream>

class Zombies
{
public:
	Zombies();
	~Zombies();

	int addZombie();
	Zombie getZombie(int);

	int count();

	void setTexture(const char*);

private:
	std::vector<Zombie> m_list;
	sf::Texture m_tex;
	sf::Sprite m_sprite;
};


#pragma once
#include "Character.h"
#include "SFML\Graphics.hpp"

class Zombies
{
public:
	Zombies();
	~Zombies();

	int addZombie();
	Character getZombie(int);

	int count();

	void setTexture(const char*);

private:
	std::vector<Character> m_list;
	sf::Texture m_tex;
	sf::Sprite m_sprite;
};


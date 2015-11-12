#pragma once
#include "Character.h"

class Zombie: public Character
{
public:
	Zombie();
	~Zombie();

	sf::Vector2f think();
};


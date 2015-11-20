#pragma once
#include "Character.h"
#include "normalize.h"

class Zombie: public Character
{
public:
	Zombie();
	~Zombie();

	sf::Vector2f think(Character&);
	sf::CircleShape getFOV();
private:
	virtual void update_internal(float dt);
	sf::CircleShape m_fov;
};


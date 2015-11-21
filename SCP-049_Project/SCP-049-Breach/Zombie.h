#pragma once
#include "Character.h"
#include "normalize.h"
#include <cstdlib>
#include <time.h>

class Zombie: public Character
{
public:
	Zombie();
	~Zombie();

	sf::Vector2f think(Character&);
	sf::CircleShape getFOV();
	sf::Vector2f getTarget();

	void move(sf::Vector2f, float);
private:
	virtual void update_internal(float dt);
	sf::CircleShape m_fov;
	sf::Vector2f m_target;
	float m_targetChange;
	int m_targetNext;
};


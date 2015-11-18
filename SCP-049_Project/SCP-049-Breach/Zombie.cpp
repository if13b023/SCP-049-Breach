#include "Zombie.h"

Zombie::Zombie()
{
	walkSpeed = 100.0f;
}

Zombie::~Zombie()
{
}


sf::Vector2f Zombie::think(sf::Vector2f main)
{
	sf::Vector2f tmp = normalize(main - this->getPosition());
	this->setRotation((atan2f(tmp.y, tmp.x) * 180 / 3.1415f) - 90.0f);
	return normalize(main - this->getPosition());
}

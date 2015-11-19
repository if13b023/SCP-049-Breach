#include "Zombie.h"

Zombie::Zombie()
{
	walkSpeed = 90.0f;
	m_attackSpeed = 100.0f;
}

Zombie::~Zombie()
{
}

sf::Vector2f Zombie::think(Character& main)
{
	sf::Vector2f tmp = normalize(main.getPosition() - this->getPosition());
	this->setRotation((atan2f(tmp.y, tmp.x) * 180 / 3.1415f) - 90.0f);
	return normalize(main.getPosition() - this->getPosition());
}

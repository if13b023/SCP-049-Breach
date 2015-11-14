#include "Zombie.h"

sf::Vector2f normalize2(const sf::Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

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

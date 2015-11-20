#include "Zombie.h"

Zombie::Zombie()
{
	walkSpeed = 90.0f;
	m_attackSpeed = 100.0f;

	m_fov = sf::CircleShape(200.0f, 3);
	m_fov.setFillColor(sf::Color::Red);
}

Zombie::~Zombie()
{
}

sf::Vector2f Zombie::think(Character& main)
{
	if (!getFOV().getGlobalBounds().intersects(main.getSprite().getGlobalBounds()))
		return sf::Vector2f(0, 0);

	sf::Vector2f tmp = normalize(main.getPosition() - this->getPosition());
	this->setRotation((atan2f(tmp.y, tmp.x) * 180 / 3.1415f) - 90.0f);
	return normalize(main.getPosition() - this->getPosition());
}

void Zombie::update_internal(float dt)
{
	//Attack speed
	if (m_attackCooldown > 0.0f)
	{
		m_attackCooldown -= (m_attackSpeed*dt);
	}
	//*** as
}

sf::CircleShape Zombie::getFOV()
{
	m_fov.setOrigin(200.0f, 0);
	m_fov.setPosition(position);
	m_fov.setRotation(getRotation());
	return m_fov;
}

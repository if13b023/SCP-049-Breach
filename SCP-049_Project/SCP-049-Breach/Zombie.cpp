#include "Zombie.h"

Zombie::Zombie()
{
	walkSpeed = 90.0f;
	m_attackSpeed = 100.0f;

	m_fov = sf::CircleShape(150.0f, 3);
	m_fov.setFillColor(sf::Color::Red);

	srand(time(NULL));
	m_target = getPosition();
	m_targetNext = 0;
	m_targetChange = 1;
}

Zombie::~Zombie()
{
}

sf::Vector2f Zombie::think(Character& main)
{
	if (!getFOV().getGlobalBounds().intersects(main.getSprite().getGlobalBounds()))
	{
		if (m_targetChange > m_targetNext)
		{
			m_target.y = position.y -250.0f + (rand() % 500);
			m_target.x = position.x -250.0f + (rand() % 500);
			//target = sf::Vector2f(500.0f, 500.0f);
			m_targetChange = 0;
			m_targetNext = 1000 + (rand() % 2000);
		}
		m_targetChange += 1.0f;
		//return normalize(target);
		sf::Vector2f diff = m_target - position;
		if (diff.x > -5 && diff.y > -5 && diff.x < 5 && diff.y < 5)
			return sf::Vector2f(0, 0);
	}
	else
		m_target = main.getPosition();

	sf::Vector2f tmp = normalize(m_target - this->getPosition());
	//this->setRotation((atan2f(tmp.y, tmp.x) * 180 / 3.1415f) - 90.0f);
	return tmp;
}

void Zombie::move(sf::Vector2f mov, float dt)
{
	sf::Vector2f tmp = normalize(m_target - this->getPosition());
	this->setRotation((atan2f(tmp.y, tmp.x) * 180 / 3.1415f) - 90.0f);
	Character::move(mov, dt);
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
	m_fov.setOrigin(150.0f, 0);
	m_fov.setPosition(position);
	m_fov.setRotation(getRotation());
	return m_fov;
}

sf::Vector2f Zombie::getTarget()
{
	return m_target;
}

#include "Zombie.h"

Zombie::Zombie()
	:m_hasTracked(false),
	m_breathnext(1.0f)
{
	walkSpeed = 90.0f;
	m_attackSpeed = 100.0f;
	m_attackDmg = 49.0f;

	m_fov = sf::CircleShape(150.0f, 3);
	m_fov.setFillColor(sf::Color::Red);

	//srand(time(NULL));
	m_target = getPosition();
	m_targetNext = 0;
	m_targetChange = 1;

	scream.setLoop(false);
	scream.setRelativeToListener(false);
	scream.setMinDistance(400.0f);
	scream.setAttenuation(4);
	scream.setVolume(100);
}

Zombie::~Zombie()
{
}

sf::Vector2f Zombie::think(MainCharacter& main)
{
	if (m_state == charState::Dead)
		return sf::Vector2f(0, 0);

	//if (!getFOV().getGlobalBounds().intersects(main.getSprite().getGlobalBounds()))
	if (!getFOV().getGlobalBounds().contains(main.getPosition()))
	{
		if (m_targetChange > m_targetNext)
		{
			m_target.y = position.y - 250.0f + (rand() % 500);
			m_target.x = position.x - 250.0f + (rand() % 500);
			//target = sf::Vector2f(500.0f, 500.0f);
			m_targetChange = 0;
			m_targetNext = 1000 + (rand() % 2000);
		}
		m_targetChange += 1.0f;
		//return normalize(target);
		sf::Vector2f diff = m_target - position;
		if (diff.x > -5 && diff.y > -5 && diff.x < 5 && diff.y < 5)
			return sf::Vector2f(0, 0);
		m_hasTracked = false;
	}
	else
	{
		sf::Vector2f distvec = position - main.getPosition();
		float dist = sqrtf(powf(distvec.x, 2.0f) + powf(distvec.y, 2.0f));

		if (main.getFlashlightSwitch() || m_hasTracked)
		{
			if (m_hasTracked == false)
			{
				scream.setPosition(position.x, position.y, 0);
				scream.play();
			}
			m_hasTracked = true;
			m_target = main.getPosition();
		}
	}

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

		if (m_attackCooldown < 0)
			m_attackCooldown = 0;
	}
	//*** as

	//Breath
	m_breathnext -= dt;
	if (m_breathnext < 0)
	{
		m_breathnext = 10.0f + (rand() % 10);
		int r = rand() % 4;
		breath[r].setPosition(position.x, position.y, 0);
		breath[r].play();
	}
	//*** b
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

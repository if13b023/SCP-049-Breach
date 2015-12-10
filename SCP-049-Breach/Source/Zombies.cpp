#include "Zombies.h"



Zombies::Zombies()
{
}


Zombies::~Zombies()
{
}

bool Zombies::setTexture(const char* path)
{
	if (!m_tex.loadFromFile(path))
		return false;
	m_tex.setRepeated(false);
	m_sprite.setTexture(m_tex);
	return true;
}

bool Zombies::setTexture(sf::Texture tx)
{
	m_tex = tx;
	m_sprite.setTexture(m_tex);
	return true;
}

size_t Zombies::addZombie()
{
	Zombie z;
	z.setSprite(m_tex);
	z.setPosition(sf::Vector2f(200.0f, 200.0f));
	z.setScale(0.15f);

	m_list.push_back(z);

	return m_list.size() - 1;
}

size_t Zombies::count()
{
	return m_list.size();
}

Zombie Zombies::getZombie(int index)
{
	return m_list.at(index);
}

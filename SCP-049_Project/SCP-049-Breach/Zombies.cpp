#include "Zombies.h"



Zombies::Zombies()
{
}


Zombies::~Zombies()
{
}

void Zombies::setTexture(const char* path)
{
	if (!m_tex.loadFromFile(path))
		std::cout << "Couldn't load " << path << std::endl;
	m_tex.setRepeated(false);
	m_sprite.setTexture(m_tex);
}

int Zombies::addZombie()
{
	Zombie z;
	z.setSprite(m_tex);
	z.setPosition(sf::Vector2f(200.0f, 200.0f));
	z.setScale(0.15f);

	m_list.push_back(z);

	return m_list.size() - 1;
}

int Zombies::count()
{
	return m_list.size();
}

Zombie Zombies::getZombie(int index)
{
	return m_list.at(index);
}

#include "Zombies.h"



Zombies::Zombies()
{
}


Zombies::~Zombies()
{
}

void Zombies::setTexture(const char* path)
{
	
}

int Zombies::addZombie()
{
	Character z;
	if (!z.setSprite("tex/Zombie_01.png"))
	{
		//std::cout << "Couldn't load \"tex / Zombie_01.png\"." << std::endl;
		//return 1;
		return NULL;
	}
	z.setScale(0.15f);

	m_list.push_back(z);

	return m_list.size() - 1;
}

int Zombies::count()
{
	return m_list.size();
}

Character Zombies::getZombie(int index)
{
	return m_list.at(index);
}

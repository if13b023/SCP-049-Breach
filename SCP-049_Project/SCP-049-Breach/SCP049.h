#pragma once
#include "Character.h"
#include "MainCharacter.h"
#include <cstdlib>
#include <time.h>

class SCP049 : public Character
{
public:
	SCP049();
	~SCP049();
	
	sf::Vector2f think(MainCharacter&);
	void move(sf::Vector2f, float);

	sf::Sound talk[7];
	sf::Sound knife;
protected:
	sf::Vector2f m_target;
	float m_changePos;
	int m_changeCnt;

	virtual void update_internal(float dt);
};


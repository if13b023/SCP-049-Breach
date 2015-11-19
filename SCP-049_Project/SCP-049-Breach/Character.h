#pragma once
#include <SFML\Graphics.hpp>
#include <ltbl\lighting\LightShape.h>
#include <vector>
#include "normalize.h"

class Character
{
public:
	Character();
	~Character();

	enum charState { Idle, Walk, Run, Dead };

	bool setSprite(const char*);
	bool setSprite(sf::Texture&);
	bool setSprite(sf::Sprite);
	sf::Sprite getSprite();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f);
	void setPosition(float, float);

	float getRotation();
	void setRotation(float);

	void setScale(float);

	float getStamina();
	float getWalkSpeed(float dt = 1.0f);

	void setState(charState);

	float getAttackDmg();

	sf::FloatRect getBoundingBox();

	void move(sf::Vector2f, float);

	bool collide(std::vector<std::shared_ptr<ltbl::LightShape>>&);
	bool collide(std::vector<Character>&);
	bool collide(Character&);

	void attack(Character&);
	bool damage(float);

	float getHealth();

protected:
	charState m_state;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Vector2f position;
	sf::Vector2f direction;
	float boundingSize;
	float stamina;
	float health;
	float walkSpeed;
	float m_attackDmg;
	float m_attackSpeed;

	void updateBoundingSize();
};


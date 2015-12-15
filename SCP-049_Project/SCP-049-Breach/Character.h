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
	sf::Sprite getSprite() const;

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f);
	void setPosition(float, float);

	float getRotation() const;
	void setRotation(float);

	void setScale(float);

	float getStamina() const;
	float getWalkSpeed(float dt = 1.0f);

	sf::Vector2f getDirection();

	void setState(charState);
	int getState();

	float getAttackDmg();

	sf::FloatRect getBoundingBox();

	void move(sf::Vector2f, float);

	bool collide(std::vector<std::shared_ptr<ltbl::LightShape>>&);
	bool collide(std::vector<Character>&);
	bool collide(Character&);

	void attack(Character&);
	bool damage(float);

	float getHealth() const;

	void update(float);

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
	float m_attackCooldown;

	void updateBoundingSize();
	virtual void update_internal(float);
};


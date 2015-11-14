#pragma once
#include <SFML\Graphics.hpp>

class Character
{
public:
	Character();
	~Character();

	enum charState { Idle, Walk, Run };

	bool setSprite(const char*);
	bool setSprite(sf::Texture&);
	bool setSprite(sf::Sprite);
	sf::Sprite getSprite();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f);
	void setPosition(float, float);

	sf::Vector2f getRotation();
	void setRotation(float);

	void setScale(float);

	float getStamina();
	float getWalkSpeed(float dt = 1.0f);

	void runs(float);
	void walks(float);
	void setState(charState);

	sf::FloatRect getBoundingBox();

	void move(sf::Vector2f);

protected:
	charState m_state;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Vector2f position;
	float boundingSize;
	float stamina;
	float health;
	float walkSpeed;
	void updateBoundingSize();
};


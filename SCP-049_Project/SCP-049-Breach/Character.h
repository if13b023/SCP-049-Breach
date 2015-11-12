#pragma once
#include <SFML\Graphics.hpp>

class Character
{
public:
	Character();
	~Character();

	bool setSprite(const char*);
	bool setSprite(sf::Texture);
	bool setSprite(sf::Sprite);
	sf::Sprite getSprite();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f);

	sf::Vector2f getRotation();
	void setRotation(float);

	void setScale(float);

	float getStamina();
	float getWalkSpeed();

	void runs(float);
	void walks(float);

	sf::FloatRect getBoundingBox();

	void move(sf::Vector2f);

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Vector2f position;
	float boundingSize;
	float stamina;
	float health;
	float walkSpeed;
	void updateBoundingSize();
};


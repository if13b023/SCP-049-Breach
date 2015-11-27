#pragma once
#include <SFML\Graphics.hpp>

namespace sf {
	class Sprite;
}

class GameObject
{
public:
	GameObject();
	GameObject(int);
	~GameObject();

	int ID();
	char* Name;
	sf::Vector2f Position;
	sf::Sprite* Sprite;

private:
	int m_id;
};

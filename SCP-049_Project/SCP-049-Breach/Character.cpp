#include "Character.h"



Character::Character()
	:	health(100),
		position(0, 0),
		walkSpeed(200.0f),
		stamina(100.0f)
{
}

Character::~Character()
{

}

bool Character::setSprite(const char* path)
{
	if (!m_texture.loadFromFile(path))
		return false;
	m_texture.setRepeated(false);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_texture.getSize().x, m_texture.getSize().y));
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, (m_texture.getSize().y / 2.0f));
	m_sprite.setPosition(position);

	updateBoundingSize();

	return true;
}

bool Character::setSprite(sf::Texture tex)
{
	m_texture = tex;
	m_texture.setRepeated(false);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_texture.getSize().x, m_texture.getSize().y));
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, (m_texture.getSize().y / 2.0f));
	m_sprite.setPosition(position);

	updateBoundingSize();

	return true;
}

sf::Vector2f Character::getPosition()
{
	return position;
}

void Character::setPosition(sf::Vector2f InPos)
{
	position = InPos;
	m_sprite.setPosition(position);
}

void Character::setRotation(float angle)
{
	m_sprite.setRotation(angle);
}

sf::Sprite Character::getSprite()
{
	return m_sprite;
}

void Character::setScale(float s)
{
	m_sprite.setScale(s, s);
	updateBoundingSize();
}

void Character::move(sf::Vector2f moveV)
{
	position += moveV;
	m_sprite.setPosition(position);
}

sf::FloatRect Character::getBoundingBox()
{
	sf::Vector2f halfSize(boundingSize*0.5f, boundingSize*0.5f);
	return sf::FloatRect(position-halfSize, sf::Vector2f(boundingSize, boundingSize));
}

void Character::updateBoundingSize()
{
	float	h = m_sprite.getLocalBounds().height * m_sprite.getScale().x,
		w = m_sprite.getLocalBounds().width * m_sprite.getScale().x;

	if (h < w)
		boundingSize = h;
	else
		boundingSize = w;
}

void Character::walks(float dt)
{
	walkSpeed = 200.0f;

	if(stamina < 100.0f)
		stamina += 20.0f * dt;
}

void Character::runs(float dt)
{
	if (stamina > 0)
	{
		walkSpeed = 400.0f;
		stamina -= 30.0f * dt;
	}
	else
		walkSpeed = 200.0f;
}

float Character::getStamina()
{
	return stamina;
}

float Character::getWalkSpeed()
{
	return walkSpeed;
}

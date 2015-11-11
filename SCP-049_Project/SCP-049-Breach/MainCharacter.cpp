#include "MainCharacter.h"



MainCharacter::MainCharacter()
	:	health(100),
		position(0, 0),
		walkSpeed(300.0f)
{
}

MainCharacter::~MainCharacter()
{

}

bool MainCharacter::setSprite(const char* path)
{
	if (!m_texture.loadFromFile(path))
		return false;
	m_texture.setRepeated(false);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_texture.getSize().x, m_texture.getSize().y));
	m_sprite.setOrigin(m_texture.getSize().x / 2, (m_texture.getSize().y / 2));
	m_sprite.setPosition(position);

	updateBoundingSize();

	return true;
}

sf::Vector2f MainCharacter::getPosition()
{
	return position;
}

void MainCharacter::setPosition(sf::Vector2f InPos)
{
	position = InPos;
	m_sprite.setPosition(position);
}

void MainCharacter::setRotation(float angle)
{
	m_sprite.setRotation(angle);
}

sf::Sprite MainCharacter::getSprite()
{
	return m_sprite;
}

void MainCharacter::setScale(float s)
{
	m_sprite.setScale(s, s);
	updateBoundingSize();
}

void MainCharacter::move(sf::Vector2f moveV)
{
	position += moveV;
	m_sprite.setPosition(position);
}

sf::FloatRect MainCharacter::getBoundingBox()
{
	sf::Vector2f halfSize(boundingSize*0.5f, boundingSize*0.5f);
	return sf::FloatRect(position-halfSize, sf::Vector2f(boundingSize, boundingSize));
}

void MainCharacter::updateBoundingSize()
{
	float	h = m_sprite.getLocalBounds().height * m_sprite.getScale().x,
		w = m_sprite.getLocalBounds().width * m_sprite.getScale().x;

	if (h < w)
		boundingSize = h;
	else
		boundingSize = w;
}

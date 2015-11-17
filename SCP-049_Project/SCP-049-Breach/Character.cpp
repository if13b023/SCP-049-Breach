#include "Character.h"

Character::Character()
	:	health(100),
		position(0, 0),
		walkSpeed(200.0f),
		stamina(100.0f),
		m_state(Character::Walk)
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

bool Character::setSprite(sf::Texture& tex)
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

void Character::setPosition(float x, float y)
{
	position = sf::Vector2f(x, y);
	m_sprite.setPosition(position);
}

float Character::getRotation()
{
	return m_sprite.getRotation();
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

void Character::move(sf::Vector2f moveV, float dt)
{
	moveV = normalize(moveV);
	direction = moveV;
	position += (moveV*getWalkSpeed(dt));
	m_sprite.setPosition(position);
}

bool Character::collide(std::vector<std::shared_ptr<ltbl::LightShape>>& list)
{
	
	for (int i = 0; i < list.size(); ++i)
	{
		if (getBoundingBox().intersects(list[i]->_shape.getGlobalBounds()))
		{
			return true;
		}
	}
	return false;
}

bool Character::collide(std::vector<Character>& list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		if (getBoundingBox().intersects(list[i].getBoundingBox()))
		{
			return true;
		}
	}
	return false;
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

float Character::getStamina()
{
	return stamina;
}

float Character::getWalkSpeed(float dt)
{
	switch (m_state)
	{
	case Character::Walk:
			if (stamina < 100.0f)
				stamina += 10.0f * dt;
			return 200.0f * dt;
		break;
	case Character::Run:
			if (stamina > 0)
			{
				stamina -= 30.0f * dt;
				return 400.0f * dt;
			}
			else
				return 200.0f * dt;
		break;
	}

	return 0;
}

void Character::setState(charState newState)
{
	m_state = newState;
}

#include "MainCharacter.h"



MainCharacter::MainCharacter()
	:	m_lightswitch(true),
		m_hasKey(false),
		m_attackRange(10.0f),
		m_batteryMax(180.0f),	//3min Flashlight
		m_battery(m_batteryMax)
{
	m_attackDmg = 110.0f;

	walkSpeed = 100.0f;
}


MainCharacter::~MainCharacter()
{
}

void MainCharacter::setFlashlight(std::shared_ptr<ltbl::LightPointEmission> flashlight)
{
	m_flashlight = flashlight;
}

bool MainCharacter::toogleFlashlight()
{
	if (m_lightswitch == true)
	{
		m_lightswitch = false;
		m_flashlight->_emissionSprite.setColor(sf::Color::Black);
	}
	else
	{
		m_lightswitch = true;
		float value = (m_battery / m_batteryMax) * 255;
		m_flashlight->_emissionSprite.setColor(sf::Color(value, value, value));
	}
	return m_lightswitch;
}

bool MainCharacter::getFlashlightSwitch()
{
	return m_lightswitch;
}

void MainCharacter::move(sf::Vector2f pos, float dt)
{
	if (m_state == Character::Dead)
		return;
	Character::move(pos, dt);
	m_flashlight->_emissionSprite.setPosition(position);
}

sf::Vector2f MainCharacter::getViewPoint()
{
	return m_viewPoint;
}

void MainCharacter::update_internal(float dt)
{
	//Health regen
	if (health < 100.0f && m_state != Character::Dead)
		health += (1.0f*dt);
	//*** hr

	//AttackCooldown
	//m_attackCooldown = 0.0f;
	if (m_attackCooldown > 0.0f)
	{
		m_attackCooldown -= (m_attackSpeed*dt);
	}
	//*** ac

	//GotHit Cooldown
	if (m_gotHit > 0)
		m_gotHit -= dt;
	//***

	//Adjust Viewpoint
	float rotation = m_sprite.getRotation() + 90.0f;
	float dist = 100.0f;
	m_viewPoint.x = (cosf((rotation*M_PI) / 180.0f)) * dist;
	m_viewPoint.y = (sinf((rotation*M_PI) / 180.0f)) * dist;
	//***

	//Flashlight
	if (m_lightswitch && (m_battery / m_batteryMax) > 0.1f)
	{
		m_battery -= dt;
		float value = (m_battery / m_batteryMax) * 255;
		m_flashlight->_emissionSprite.setColor(sf::Color(value, value, value));
	}
	//***
}

bool MainCharacter::hasKey()
{
	return m_hasKey;
}

void MainCharacter::collectKey()
{
	m_hasKey = true;
}

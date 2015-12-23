#include "MainCharacter.h"



MainCharacter::MainCharacter()
	:	m_lightswitch(true),
		m_hasKey(false),
		m_attackRange(10.0f)
{
	m_attackDmg = 110.0f;
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
		m_flashlight->_emissionSprite.setColor(sf::Color::White);
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
}

bool MainCharacter::hasKey()
{
	return m_hasKey;
}

void MainCharacter::collectKey()
{
	m_hasKey = true;
}

#include "MainCharacter.h"



MainCharacter::MainCharacter()
{
	m_lightswitch = true;
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
	Character::move(pos, dt);
	m_flashlight->_emissionSprite.setPosition(position);
}

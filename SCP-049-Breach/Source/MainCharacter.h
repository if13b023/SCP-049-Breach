#pragma once
#include "Character.h"
#include <ltbl\lighting\LightPointEmission.h>
class MainCharacter :
	public Character
{
public:
	MainCharacter();
	~MainCharacter();

	void setFlashlight(std::shared_ptr<ltbl::LightPointEmission>);
	std::shared_ptr<ltbl::LightPointEmission> getFlashlight();
	bool toogleFlashlight();
	bool getFlashlightSwitch();

	bool hasKey();
	void collectKey();

	void move(sf::Vector2f pos, float);

private:
	std::shared_ptr<ltbl::LightPointEmission> m_flashlight;
	bool m_lightswitch;
	virtual void update_internal(float);
	bool m_hasKey;
	float m_attackRange;
};


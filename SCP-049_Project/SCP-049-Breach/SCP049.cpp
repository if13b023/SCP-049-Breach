#include "SCP049.h"



SCP049::SCP049()
	:m_changePos(1.0f),
	m_changeCnt(0),
	m_talkEvent(false)
{
	walkSpeed = 20.0f;
	m_attackDmg = 150.0f;

	knife.setLoop(true);
	knife.setRelativeToListener(false);
	knife.setMinDistance(100.0f);
	knife.setAttenuation(4);
	knife.setVolume(80);
}


SCP049::~SCP049()
{
}

sf::Vector2f SCP049::think(MainCharacter& main)
{
	m_target = main.getPosition();
	sf::Vector2f tmp = normalize(m_target - this->getPosition());
	float rot = (atan2f(tmp.y, tmp.x) * 180 / M_PI) - 90.0f;
	this->setRotation(rot);

	float face = main.getRotation() - rot;

	if (face > 360.0f)
		face -= 360.0f;
	else if (face < 0.0f)
		face += 360.0f;

	face -= 180.0f;
	
	sf::Vector2f distvec = position - main.getPosition();
	float dist = sqrtf(powf(distvec.x, 2.0f) + powf(distvec.y, 2.0f));

	if(dist < 200.0f)
	{
		if (m_talkEvent == false && m_enabled)
		{
			int r = 1 + (rand() % 6);
			talk[r].play();
			m_talkEvent = true;
		}
	}
	else
		m_talkEvent = false;

	if (dist > 700.0f)
	{
		if (m_changeCnt > m_changePos && (face > 50.0f || face < -50.0f))
		{
			float a;
			int t = 80;
			a = main.getRotation() + t + (rand() % (360 - (2 * t)));

			//std::cout << "position changed! > " << main.getRotation() << " :: " << a << std::endl;

			float x = m_target.x - (sinf((a*M_PI) / 180.0f) * 400.0f);
			float y = m_target.y + (cosf((a*M_PI) / 180.0f) * 400.0f);

			this->setPosition(x, y);

			m_changeCnt = 0;
			m_changePos = 100.0f + (rand() % 200);

			if (main.hasKey())
			{
				m_changePos *= 2.0f;
				walkSpeed = 40.0f;
			}
			else
				walkSpeed = 20.0f;
		}
	}
	m_changeCnt++;

	return m_target - this->position;
}

void SCP049::move(sf::Vector2f mov, float dt)
{
	sf::Vector2f tmp = normalize(m_target - this->getPosition());
	Character::move(mov, dt);
}

void SCP049::update_internal(float dt)
{
	knife.setPosition(position.x, position.y, 0);
}



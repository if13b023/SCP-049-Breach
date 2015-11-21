#include <ltbl\lighting\LightSystem.h>

#include <SFML/Graphics.hpp>

#include <sstream>
#include <cassert>
#include <cstdlib>

#include <iostream>
#include <thread>

#include "FileIO.h"
#include "Character.h"
#include "Zombie.h"
#include "MainCharacter.h"

#include "normalize.h"

int main(int argc, char* argv)
{
	//argument parsing
	for (int i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;
	//***
	
	//INIT
	sf::VideoMode vm;
	vm.width = 1280;
	vm.height = 720;
	vm.bitsPerPixel = 32;
	assert(vm.isValid());

	std::list<std::thread> threads;

	//creating the window
	sf::RenderWindow window;
	uint32_t style = sf::Style::Close | sf::Style::Titlebar;
#define FULLSCREEN 0
#if FULLSCREEN
	vm.height = 1080;
	vm.width = 1920;
	style = sf::Style::Fullscreen;
#endif
	window.create(vm, "SCP-049-Breach", style);
	window.setMouseCursorVisible(false);

	//loading Textures and creating Sprites
	sf::Texture bgTex;
	//assert(bgTex.loadFromFile("data/Level1_b.jpg"));
	if (!bgTex.loadFromFile("data/Level1_2_big.jpg"))
		abort();
	bgTex.setRepeated(false);

	sf::Sprite bgSprite(bgTex);
	bgSprite.setTextureRect(sf::IntRect(0, 0, bgTex.getSize().x, bgTex.getSize().y));
	bgSprite.setPosition(0, 0);

	sf::Texture bgTopTex;
	assert(bgTopTex.loadFromFile("data/Level1_2_big_top.png"));
	bgTopTex.setRepeated(false);

	sf::Sprite bgTopSprite(bgTopTex);
	bgTopSprite.setTextureRect(sf::IntRect(0, 0, bgTopTex.getSize().x, bgTopTex.getSize().y));
	bgTopSprite.setPosition(0, 0);

	sf::Shader unshadowShader;
	sf::Shader lightOverShapeShader;
	unshadowShader.loadFromFile("resources/unshadowShader.vert", "resources/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("resources/lightOverShapeShader.vert", "resources/lightOverShapeShader.frag");

	sf::Texture penumbraTex;
	penumbraTex.loadFromFile("resources/penumbraTexture.png");
	penumbraTex.setSmooth(true);

	sf::Texture flashLightTex;
	flashLightTex.loadFromFile("resources/flashlightTexture_g.png");
	flashLightTex.setSmooth(true);

	//GUI
	sf::Font font;
	font.loadFromFile("resources/OpenSans-Regular.ttf");
	sf::Text healthBar("Test", font, 75);
	healthBar.setColor(sf::Color::White);
	healthBar.setPosition(20.0f, 10.0f);
	//*** gui

	//LightSystem
	ltbl::LightSystem ls;
	sf::Color ambientColor = sf::Color(100, 100, 100, 255);
	ls.create(sf::FloatRect(-10.0f, -10.0f, 10.0f, 10.0f), window.getSize(), penumbraTex, unshadowShader, lightOverShapeShader);
	ls._ambientColor = ambientColor;

	std::vector<std::shared_ptr<ltbl::LightShape>> lightShapes;
	lightShapes.reserve(256);
	//***	LightSystem

	//Flashlight
	std::shared_ptr<ltbl::LightPointEmission> light = std::make_shared<ltbl::LightPointEmission>();

	light->_emissionSprite.setOrigin(sf::Vector2f(10.0f, 10.0f));
	light->_emissionSprite.setTexture(flashLightTex);
	light->_emissionSprite.setScale(sf::Vector2f(1.5f, 1.5f));
	light->_emissionSprite.setColor(sf::Color::White);
	light->_emissionSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	ls.addLight(light);
	//** fl

	/*sf::Texture characterTex;
	characterTex.loadFromFile("tex/Sprite_01.png");

	sf::Sprite characterSprite(characterTex);
	characterSprite.setOrigin(characterTex.getSize().x / 2, (characterTex.getSize().y / 2));
	characterSprite.setScale(0.15, 0.15);*/
	MainCharacter mainChar;
	if (!mainChar.setSprite("tex/Sprite_01.png"))
	{
		std::cout << "Couldn't load \"tex / Sprite_01.png\"." << std::endl;
		return 1;
	}
	mainChar.setScale(0.15f);
	mainChar.setFlashlight(light);

	Zombie z;
	if (!z.setSprite("tex/Zombie_01.png"))
	{
		std::cout << "Couldn't load \"tex / Sprite_01.png\"." << std::endl;
		return 1;
	}
	z.setScale(0.15f);
	z.setPosition(100.0f, 100.0f);
	//***	loading Textures and creating Sprites

	//Creating Zombies
	//Zombies zombies;
	std::vector<Zombie> zombies;
	zombies.reserve(16);
	/*if (!zombies.setTexture("tex/Zombie_01.png"))
	{
		std::cout << "Couldn't load \"tex / Zombie_01.png\"" << std::endl;
	}*/
	for (int i = 0; i < 10; ++i)
	{
		z.setPosition(z.getPosition() + sf::Vector2f(z.getSprite().getGlobalBounds().width + 50.0f , 0));
		zombies.push_back(z);
	}
	//*** cz

	sf::Event eve;

	bool quit = false;

	sf::Clock clock;
	float dt = 0.016f;

	int cnt = 0;

	sf::View view = window.getDefaultView();
	float zoomFactor = 1.0f;
	view.zoom(0.5f);
	mainChar.setPosition(view.getCenter());
	FileWriter fw;
	bool show_fps = false;

	window.setVerticalSyncEnabled(false);

	fw.LoadLightShapesFromFile("data/Level1_2_big.jpg.txt", lightShapes, ls);
	//***	INIT END

	while (!quit)
	{
		clock.restart();

		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		while (window.pollEvent(eve))
		{
			if (eve.type == sf::Event::Closed)
			{
				quit = true;
				break;
			}

			if (eve.type == sf::Event::MouseWheelScrolled)
			{
				//std::cout << "wheel delta > " << eve.mouseWheelScroll.delta << std::endl;

				if (eve.mouseWheelScroll.delta < 0 && zoomFactor < 2.0f)
				{
					zoomFactor += 0.1f;
					view.zoom(1.05f);
				}
				else if (eve.mouseWheelScroll.delta > 0 && zoomFactor > 0.5f)
				{
					zoomFactor -= 0.1f;
					view.zoom(0.95f);
				}
				break;
			}

			if (eve.type == sf::Event::MouseButtonPressed)
			{
				if (eve.mouseButton.button == sf::Mouse::Left)
				{
					mainChar.toogleFlashlight();
				}
				else if (eve.mouseButton.button == sf::Mouse::Right)
				{

				}
				break;
			}

			if (eve.type == sf::Event::KeyPressed)
			{
				switch (eve.key.code)
				{
				case sf::Keyboard::T:
					show_fps = !show_fps;
					std::cout << "show_fps> " << show_fps << std::endl;
					break;

				case sf::Keyboard::Escape:
					quit = true;
					break;
				}
			}
		}

		if (mainChar.getState() != Character::Dead)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				mainChar.setState(Character::Run);
			else
				mainChar.setState(Character::Walk);
		}

		sf::Vector2f moveVec;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			moveVec.x = -1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			moveVec.x = 1.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			moveVec.y = -1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			moveVec.y = 1.0f;

		//MainCharacter CollisionDetection
		bool collided = false;
		mainChar.move(moveVec, dt);
		sf::FloatRect bgBounds = bgSprite.getGlobalBounds();
		float border = 30.0f;
		bgBounds.height -= border;
		bgBounds.width -= border;
		bgBounds.left += border;
		bgBounds.top += border;
		mainChar.update(dt);
		if(!bgBounds.contains(mainChar.getPosition()))
			mainChar.move(-moveVec, dt);
		else
		{
			if (mainChar.collide(lightShapes))
			{
				collided = true;
				mainChar.move(-moveVec, dt);
			}

			for (int i = 0; i < zombies.size() && !collided; ++i)
			{
				if (mainChar.collide(zombies.at(i)))
					mainChar.move(-moveVec, dt);
			}
		}
		//*** mccd

		//AI
		collided = false;
		for (int i = 0; i < zombies.size(); ++i)
		{
			zombies.at(i).update(dt);
			collided = false;
			sf::Vector2f zmov = zombies.at(i).think(mainChar) * dt * zombies.at(i).getWalkSpeed();

			zombies.at(i).move(zmov, dt);

			if (zombies.at(i).collide(lightShapes) || !bgBounds.contains(zombies.at(i).getPosition()))
			{
				collided = true;
				zombies.at(i).move(-zmov, dt);
			}

			for (int j = 0; j < zombies.size() && !collided; ++j)
			{
				if (i != j && zombies.at(i).getBoundingBox().intersects(zombies.at(j).getBoundingBox()))
					zombies.at(i).move(-zmov, dt);
			}

			if (!collided && zombies.at(i).collide(mainChar))
			{
				//zombies.at(i).attack(mainChar);
				zombies.at(i).move(-zmov, dt);
			}
		}
		//*** ai

		view.setCenter(mainChar.getPosition());
		healthBar.setString(std::to_string(static_cast<int>(mainChar.getHealth())));

		//Flashlight Rotation
		sf::Vector2f v = light->_emissionSprite.getPosition() - mousePos;
		light->_emissionSprite.setRotation((atan2f(v.y, v.x) * 180 / 3.1415f) + 135.0f);
		mainChar.setRotation((atan2f(v.y, v.x) * 180 / 3.1415f) + 90.0f);
		//** fr

		window.clear();

		window.setView(view);

		window.draw(bgSprite);

		for (int i = 0; i < zombies.size(); ++i)
		{
			window.draw(zombies.at(i).getSprite());
			//window.draw(zombies.at(i).getFOV());
			/*sf::CircleShape zc(10.0f, 3);
			zc.setFillColor(sf::Color::Red);
			zc.setPosition(zombies.at(i).getTarget());
			window.draw(zc);*/
		}
		window.draw(mainChar.getSprite());

		//window.draw(bgTopSprite);

		ls.render(view, unshadowShader, lightOverShapeShader);
		sf::Sprite lightSprite;
		lightSprite.setTexture(ls.getLightingTexture());

		sf::RenderStates lightRenderStates;
		lightRenderStates.blendMode = sf::BlendMultiply;

		window.setView(window.getDefaultView());
		window.draw(lightSprite, lightRenderStates);

		window.draw(healthBar);
		window.setView(view);

		window.display();

		dt = clock.getElapsedTime().asSeconds();

		cnt++;
		if (cnt % 100 == 0 && show_fps)
		{
			std::cout << dt << " - " << (1.0f / dt) << " - " << (atan2f(v.y, v.x) * 180 / 3.1415f) << " - " << mainChar.getStamina() << std::endl;
			cnt = 0;
		}
	}

	for (auto& t : threads)
		t.join();

	window.close();
	return 0;
}

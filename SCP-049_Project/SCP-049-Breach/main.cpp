#include <ltbl\lighting\LightSystem.h>

#include <SFML/Graphics.hpp>

#include <sstream>
#include <cassert>
#include <cstdlib>

#include <iostream>
#include <thread>

#include "LightShapeMaker.h"
#include "FileIO.h"
#include "MainCharacter.h"

bool ViewportLock(const sf::View& v, const sf::Vector2f& pos)
{
	sf::Vector2f topLeft = v.getCenter() - (v.getSize() / 2.0f);
	std::cout << "debug -> \t" << topLeft.x << ":" << topLeft.y << std::endl;
	//if ((v.left <= pos.x) || (v.top <= pos.y))
	//	return false;
	return true;
}

sf::Vector2f normalize(const sf::Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

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
	if (!bgTex.loadFromFile("data/Level1_b.jpg"))
		abort();
	bgTex.setRepeated(false);

	sf::Sprite bgSprite(bgTex);
	bgSprite.setTextureRect(sf::IntRect(0, 0, bgTex.getSize().x, bgTex.getSize().y));
	bgSprite.setPosition(0, 0);

	sf::Texture bgTopTex;
	assert(bgTopTex.loadFromFile("data/Level1_big_top.png"));
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
	//***	loading Textures and creating Sprites

	//LightSystem
	ltbl::LightSystem ls;
	sf::Color ambientColor = sf::Color(100, 100, 100, 255);
	ls.create(sf::FloatRect(-1000.0f, -1000.0f, 1000.0f, 1000.0f), window.getSize(), penumbraTex, unshadowShader, lightOverShapeShader);
	ls._ambientColor = ambientColor;

	std::shared_ptr<ltbl::LightPointEmission> light = std::make_shared<ltbl::LightPointEmission>();

	light->_emissionSprite.setOrigin(sf::Vector2f(10.0f, 10.0f));
	light->_emissionSprite.setTexture(flashLightTex);
	light->_emissionSprite.setScale(sf::Vector2f(1.5f, 1.5f));
	light->_emissionSprite.setColor(sf::Color(200, 200, 200));
	light->_emissionSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	ls.addLight(light);

	std::vector<std::shared_ptr<ltbl::LightShape>> lightShapes;

	LightShapeMaker lsm;
	//***	LightSystem

	sf::Event eve;

	bool quit = false;

	sf::Clock clock;
	float dt = 0.016f;

	int cnt = 0;

	sf::View view = window.getDefaultView();
	view.zoom(0.5f);
	mainChar.setPosition(view.getCenter());
	FileWriter fw;
	bool show_fps = false;

	window.setVerticalSyncEnabled(false);

	fw.LoadLightShapesFromFile("Level1.txt", lightShapes, ls);
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

				if (eve.mouseWheelScroll.delta < 0 && ViewportLock(view, bgSprite.getPosition()))
					view.zoom(1.1f);

				if (eve.mouseWheelScroll.delta > 0)
					view.zoom(0.9f);
				break;
			}

			if (eve.type == sf::Event::MouseButtonPressed)
			{
				if (eve.mouseButton.button == sf::Mouse::Left)
				{
					/*int index = lsm.addHexToList(lightShapes, mousePos);
					std::cout << "mouse x_y: " << mousePos.x << "_" << mousePos.y << "; index = " << lightShapes.size() << std::endl;
					ls.addShape(lightShapes.at(index));*/
					if (!lsm.isActive())
					{
						lsm.begin(lightShapes, 64);
						lsm.setPosition(mousePos);
					}
					else
						lsm.addPoint(mousePos);
				}
				else if (eve.mouseButton.button == sf::Mouse::Right)
				{
					if (lsm.isActive())
					{
						size_t index = lsm.finish();
						if (index != -1)
							ls.addShape(lightShapes.at(index));
					}
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

		float moveSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			moveSpeed = 300.0f;
		else
			moveSpeed = 200.0f;

		sf::Vector2f moveVec;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			moveVec.x = -1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			moveVec.x = 1.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			moveVec.y = -1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			moveVec.y = 1.0f;

		moveVec = normalize(moveVec);
		moveVec *= (moveSpeed*dt);

		//view.move(moveVec);

		//CollisionDetection
		mainChar.move(moveVec);
		for (int i = 0; i < lightShapes.size(); ++i)
		{
			if (mainChar.getBoundingBox().intersects(lightShapes[i]->_shape.getGlobalBounds()))
			{
				//std::cout << "Collision!" << dt << std::endl;
				mainChar.move(-moveVec);
				//view.move(-moveVec);
			}
		}
		//*** CD

		view.setCenter(mainChar.getPosition());
		light->_emissionSprite.setPosition(mainChar.getPosition());

		//Flashlight Rotation
		sf::Vector2f v = light->_emissionSprite.getPosition() - mousePos;
		light->_emissionSprite.setRotation((atan2f(v.y, v.x) * 180 / 3.1415f) + 135.0f);
		mainChar.setRotation((atan2f(v.y, v.x) * 180 / 3.1415f) + 90.0f);

		window.clear();

		window.setView(view);

		window.draw(bgSprite);

		window.draw(mainChar.getSprite());

		window.draw(bgTopSprite);

		ls.render(view, unshadowShader, lightOverShapeShader);
		sf::Sprite lightSprite;
		lightSprite.setTexture(ls.getLightingTexture());

		sf::RenderStates lightRenderStates;
		lightRenderStates.blendMode = sf::BlendMultiply;

		window.setView(window.getDefaultView());
		window.draw(lightSprite, lightRenderStates);
		window.setView(view);

		window.display();

		dt = clock.getElapsedTime().asSeconds();

		cnt++;
		if (cnt % 10 == 0 && show_fps)
		{
			std::cout << mainChar.getPosition().x << " " << mainChar.getPosition().y << std::endl;
			std::cout << dt << " - " << (1.0f / dt) << " - " << (atan2f(v.y, v.x) * 180 / 3.1415f) << " - " << std::endl;
			cnt = 0;
		}
	}

	for (auto& t : threads)
		t.join();

	window.close();
	return 0;
}

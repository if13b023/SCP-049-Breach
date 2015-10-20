#include <ltbl\lighting\LightSystem.h>

#include <SFML/Graphics.hpp>

#include <sstream>
#include <cassert>
#include <cstdlib>

#include <iostream>
#include <thread>

#include "LightShapeMaker.h"
#include "FileIO.h"

int main()
{
	sf::VideoMode vm;
	vm.width = 1280;
	vm.height = 720;
	vm.bitsPerPixel = 32;
	assert(vm.isValid());

	std::list<std::thread> threads;

	sf::RenderWindow window;
	window.create(vm, "Light Demo");

	sf::Texture bgImg;
	//assert(bgImg.loadFromFile("data/background.png"));
	if (!bgImg.loadFromFile("data/Level1_b.jpg"))
		abort();
	bgImg.setRepeated(false);

	sf::Sprite bgSprite(bgImg);
	bgSprite.setTextureRect(sf::IntRect(0, 0, bgImg.getSize().x, bgImg.getSize().y));

	sf::Shader unshadowShader;
	sf::Shader lightOverShapeShader;
	unshadowShader.loadFromFile("resources/unshadowShader.vert", "resources/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("resources/lightOverShapeShader.vert", "resources/lightOverShapeShader.frag");

	sf::Texture penumbraTexture;
	penumbraTexture.loadFromFile("resources/penumbraTexture.png");
	penumbraTexture.setSmooth(true);

	sf::Texture pointLightTexture;
	pointLightTexture.loadFromFile("resources/flashlightTexture_g.png");
	pointLightTexture.setSmooth(true);

	ltbl::LightSystem ls;
	ls.create(sf::FloatRect(-1000.0f, -1000.0f, 1000.0f, 1000.0f), window.getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);

	std::shared_ptr<ltbl::LightPointEmission> light = std::make_shared<ltbl::LightPointEmission>();

	//light->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
	light->_emissionSprite.setOrigin(sf::Vector2f(0, 0));
	light->_emissionSprite.setTexture(pointLightTexture);
	//light->_emissionSprite.setScale(sf::Vector2f(12.0f, 12.0f));
	light->_emissionSprite.setScale(sf::Vector2f(1.0f, 1.0f));
	light->_emissionSprite.setColor(sf::Color(200, 200, 200));
	light->_emissionSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	ls.addLight(light);

	std::map<int, std::shared_ptr<ltbl::LightShape>> lightShapes;
	/*
	lightShapes[1] = std::make_shared<ltbl::LightShape>();

	lightShapes[1]->_shape.setPointCount(6);

	lightShapes[1]->_shape.setPoint(0, sf::Vector2f(25, -43));
	lightShapes[1]->_shape.setPoint(1, sf::Vector2f(-25, -43));
	lightShapes[1]->_shape.setPoint(2, sf::Vector2f(-50, 0));
	lightShapes[1]->_shape.setPoint(3, sf::Vector2f(-25, 43));
	lightShapes[1]->_shape.setPoint(4, sf::Vector2f(25, 43));
	lightShapes[1]->_shape.setPoint(5, sf::Vector2f(50, 0));

	lightShapes[1]->_shape.setPosition(1140.0f, 236.0f);

	lightShapes[1]->_renderLightOverShape = true;

	ls.addShape(lightShapes.at(1));*/

	LightShapeMaker lsm;

	sf::Event eve;

	bool quit = false;

	sf::Clock clock;
	float dt = 0.016f;

	int cnt = 0;

	sf::View view = window.getDefaultView();
	//view.setCenter(sf::Vector2f(bgImg.getSize().x*0.5f, bgImg.getSize().y*0.5f));
	//view.zoom(0.5f);
	FileWriter fw;
	bool show_fps = true;

	window.setVerticalSyncEnabled(true);
	float moveSpeed = 500.0f;

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
				std::cout << "wheel delta > " << eve.mouseWheel.delta << std::endl;
				view.zoom(1.0f + eve.mouseWheel.delta);
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
						int index = lsm.finish();
						if(index != -1)
							ls.addShape(lightShapes.at(index));
					}
				}
				break;
			}

			if (eve.type == sf::Event::KeyPressed)
			{
				switch (eve.key.code)
				{
					case sf::Keyboard::P:
						std::cout << "saving map...\n";
						fw.SaveLightShapes("Level1.txt", lightShapes);
					break;

					case sf::Keyboard::T:
						show_fps = !show_fps;
						std::cout << "show_fps> " << show_fps << std::endl;	
					break;

					case sf::Keyboard::Escape:
						quit = true;
					break;

					case sf::Keyboard::O:
						fw.LoadLightShapesFromFile("Level1.txt", lightShapes, ls);
					break;
				}
			}

			if (eve.type == sf::Event::KeyReleased)
			{
				switch (eve.key.code)
				{
					default:
					break;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			moveSpeed = 1000.0f;
		else
			moveSpeed = 500.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			view.move(sf::Vector2f(-moveSpeed*dt, 0.0f));
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			view.move(sf::Vector2f(moveSpeed*dt, 0.0f));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			view.move(sf::Vector2f(0.0f, -moveSpeed*dt));
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			view.move(sf::Vector2f(0.0f, moveSpeed*dt));

		//std::cout << mousePos.x << " " << mousePos.y << std::endl;

		//light->_emissionSprite.setPosition(sf::Vector2f(mousePos.x, mousePos.y));
		light->_emissionSprite.setPosition(view.getCenter());

		//Flashlight Rotation
		sf::Vector2f v = light->_emissionSprite.getPosition() - mousePos;
		light->_emissionSprite.setRotation((atan2f(v.y, v.x) * 180 / 3.1415f)+135.0f);

		window.clear();

		window.draw(bgSprite);
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
		if (cnt%10 == 0 && show_fps)
		{
			std::cout << dt << " - " << (1.0f / dt) << " - "  << (atan2f(v.y,v.x) * 180 / 3.1415f) << std::endl;
			cnt = 0;
		}
	}

	for (auto& t : threads)
		t.join();

	window.close();
	return 0;
}

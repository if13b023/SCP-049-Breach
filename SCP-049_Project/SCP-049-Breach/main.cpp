#include <ltbl\lighting\LightSystem.h>

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include <sstream>
#include <cassert>
#include <cstdlib>

#include <iostream>
#include <thread>
#include <time.h>
#include <Windows.h>

#include "FileIO.h"
#include "Character.h"
#include "Zombie.h"
#include "MainCharacter.h"
#include "SCP049.h"

#include "normalize.h"

enum GameState{Running, Win, Lose, Paused, Intro};

int main(int argc, char** argv)
{
	//FreeConsole();

	GameState state = Intro;

	//argument parsing
	//for (int i = 0; i < argc; i++)
	//	std::cout << argv[i] << std::endl;
	//***

	//INIT
	sf::VideoMode vm;
	vm.width = 1280;
	vm.height = 720;
	vm.bitsPerPixel = 32;
	//assert(vm.isValid());
	if (!vm.isValid())
		abort();

	std::list<std::thread> threads;
	srand(static_cast<unsigned int>(time(NULL)));

	FileWriter fw;

	//creating the window
	sf::RenderWindow window;
	uint32_t style = sf::Style::Close | sf::Style::Titlebar;

	if (argc > 1)
	{
		if (strcmp(argv[1], "-fullscreen") == 0)
		{
			vm.height = 1080;
			vm.width = 1920;
			style = sf::Style::Fullscreen;
		}

	}
	window.create(vm, "SCP-049-Breach", style);
	window.setMouseCursorVisible(false);

	//Loading Screen
	sf::Font font_default;
	font_default.loadFromFile("resources/OpenSans-Regular.ttf");

	sf::Text loading("loading...", font_default, 30);
	loading.setColor(sf::Color::White);
	loading.setPosition(10.0f, 10.0f);
	window.draw(loading);
	window.display();

	std::string introText(fw.LoadText("data/intro.txt"));
	//** ls

	//Music
	sf::Music music;
	music.openFromFile("sounds/hauntyou.ogg");
	music.setLoop(true);
	music.setVolume(60);
	music.play();
	//***

	//loading Textures and creating Sprites
	sf::Texture bgTex;
	//assert(bgTex.loadFromFile("data/Level1_b.jpg"));
	if (!bgTex.loadFromFile("data/Level1_3_big.jpg"))
		abort();
	bgTex.setRepeated(false);

	sf::Sprite bgSprite(bgTex);
	bgSprite.setTextureRect(sf::IntRect(0, 0, bgTex.getSize().x, bgTex.getSize().y));
	bgSprite.setPosition(0, 0);

	sf::Texture bgTopTex;
	//assert(bgTopTex.loadFromFile("data/Level1_2_big_top.png"));
	if (!bgTopTex.loadFromFile("data/Level1_3_big_top.png"))
		abort();
	bgTopTex.setRepeated(false);

	sf::Sprite bgTopSprite(bgTopTex);
	bgTopSprite.setTextureRect(sf::IntRect(0, 0, bgTopTex.getSize().x, bgTopTex.getSize().y));
	bgTopSprite.setPosition(0, 0);

	sf::Texture bgLightTex;
	//assert(bgLightTex.loadFromFile("data/Level1_2_big_light.jpg"));
	if (!bgLightTex.loadFromFile("data/Level1_3_big_light.jpg"))
		abort();
	bgLightTex.setRepeated(false);
	bgLightTex.setSmooth(false);

	sf::Sprite bgLightSprite(bgLightTex);
	bgLightSprite.setTextureRect(sf::IntRect(0, 0, bgLightTex.getSize().x, bgLightTex.getSize().y));
	bgLightSprite.setPosition(0, 0);

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

	sf::Texture pointLightTex;
	pointLightTex.loadFromFile("resources/pointLightTexture.png");
	pointLightTex.setSmooth(true);

	sf::Texture KeyTex;
	KeyTex.loadFromFile("tex/KeyCard.png");
	KeyTex.setSmooth(true);

	//Sounds
	sf::SoundBuffer soundbuffer[256];

	soundbuffer[0].loadFromFile("sounds/flashlight02_on.wav");	//Flashlight On
	soundbuffer[1].loadFromFile("sounds/flashlight02_off.wav");	//Flashlight Off
	soundbuffer[2].loadFromFile("sounds/footgrass_01.wav");	//Footstep1
	soundbuffer[3].loadFromFile("sounds/footgrass_02.wav");	//Footstep2
	soundbuffer[4].loadFromFile("sounds/footgrass_03.wav");	//Footstep3
	soundbuffer[5].loadFromFile("sounds/footgrass_04.wav");	//Footstep4

	sf::Sound flashlightSnd[2];
	flashlightSnd[0].setBuffer(soundbuffer[0]);
	flashlightSnd[1].setBuffer(soundbuffer[1]);
	flashlightSnd[0].setRelativeToListener(true);
	flashlightSnd[1].setRelativeToListener(true);

	sf::Sound footstepSnd[4];
	float footstepCnt = 0;

	for (int i = 0; i < 4; ++i)
	{
		footstepSnd[i].setBuffer(soundbuffer[i+2]);
		footstepSnd[i].setRelativeToListener(true);
	}

	footstepSnd[0].setBuffer(soundbuffer[2]);
	footstepSnd[1].setBuffer(soundbuffer[3]);
	footstepSnd[2].setBuffer(soundbuffer[4]);
	footstepSnd[3].setBuffer(soundbuffer[5]);
	
	for (int i = 0; i < 4; ++i)
		footstepSnd[i].setVolume(20.f);

	soundbuffer[6].loadFromFile("sounds/zombie_moan.wav");

	soundbuffer[7].loadFromFile("sounds/generator.wav");
	sf::Sound generator[2];
	
	for (int i = 0; i < 2; ++i)
	{
		generator[i].setBuffer(soundbuffer[7]);
		generator[i].setLoop(true);
		generator[i].setVolume(70);
		generator[i].setRelativeToListener(false);
		generator[i].setMinDistance(300.0f);
		generator[i].setAttenuation(4);
		generator[i].play();
	}

	generator[0].setPosition(2700, 2700, 0);
	generator[1].setPosition(3240, 2540, 0);
	
	soundbuffer[8].loadFromFile("sounds/pg_patient.wav");
	soundbuffer[9].loadFromFile("sounds/pg_cure.wav");
	soundbuffer[10].loadFromFile("sounds/pg_disease.wav");
	soundbuffer[11].loadFromFile("sounds/pg_findyou.wav");
	soundbuffer[12].loadFromFile("sounds/pg_help.wav");
	soundbuffer[13].loadFromFile("sounds/pg_hide.wav");
	soundbuffer[14].loadFromFile("sounds/pg_run.wav");

	sf::Sound kill;
	bool died = false;
	soundbuffer[15].loadFromFile("sounds/pg_kill.wav");
	kill.setBuffer(soundbuffer[15]);

	soundbuffer[16].loadFromFile("sounds/pg_knife.wav");
	soundbuffer[17].loadFromFile("sounds/zombie_scream.wav");
	//***
	
	//Key
	sf::Sprite Key;
	Key.setTexture(KeyTex);
	Key.setScale(0.1f, 0.1f);
	std::cout << "Key: " << Key.getPosition().x << ":" << Key.getPosition().y << std::endl;
	
	{
		std::vector<sf::Vector2f> keySpawns;
		fw.LoadSpawnPoints("data/keyspawns.txt", keySpawns);
		int r = rand() % keySpawns.size();
		Key.setPosition(keySpawns.at(r));
	}
	//*** k

	//Exit
	sf::FloatRect exit(256.0f, 0, 110.0f, 50.0f);
	//*** e

	//GUI
	sf::Text healthBar("Test", font_default, 75);
	healthBar.setColor(sf::Color::White);
	healthBar.setPosition(vm.width - healthBar.getGlobalBounds().width - 20.0f, vm.height - healthBar.getGlobalBounds().height - 50.0f);

	sf::Font font_end;
	font_end.loadFromFile("resources/theend.ttf");
	sf::Text endTitle("END", font_end, 100);
	endTitle.setColor(sf::Color::White);
	//*** gui

	//LightSystem
	ltbl::LightSystem ls;
	sf::Color ambientColor = sf::Color(10, 10, 10, 255);
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

	//Pointlight
	std::shared_ptr<ltbl::LightPointEmission> lightPoint = std::make_shared<ltbl::LightPointEmission>();

	sf::Color pointColor = sf::Color(100, 100, 100, 255);

	lightPoint->_emissionSprite.setOrigin(sf::Vector2f(32.0f, 32.0f));
	lightPoint->_emissionSprite.setTexture(pointLightTex);
	lightPoint->_emissionSprite.setScale(sf::Vector2f(5.0f, 5.0f));
	lightPoint->_emissionSprite.setColor(pointColor);
	lightPoint->_emissionSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	ls.addLight(lightPoint);
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
	mainChar.setPosition(310.0f, 50.0f);
	//mainChar.setPosition(2500.0f, 2300.0f);

	Zombie z;
	if (!z.setSprite("tex/Zombie_01.png"))
	{
		std::cout << "Couldn't load \"tex / Sprite_01.png\"." << std::endl;
		return 1;
	}
	z.setScale(0.15f);
	z.setPosition(310.0f, 50.0f);
	z.breath[0].setBuffer(soundbuffer[6]);
	z.breath[0].setRelativeToListener(false);
	z.breath[0].setMinDistance(100.0f);
	z.breath[0].setAttenuation(3.0f);

	z.scream.setBuffer(soundbuffer[17]);

	sf::Texture ZombieDeadTex[2];
	
	ZombieDeadTex[0].loadFromFile("tex/Zombie_dead_01.png");
	ZombieDeadTex[1].loadFromFile("tex/Zombie_dead_02.png");
	//***	loading Textures and creating Sprites

	//Creating Zombies
	//Zombies zombies;
	std::vector<Zombie> zombies;
	const int zCount = 10;
	zombies.reserve(16);

	{
		//sf::Vector2f zPosTmp;
		std::vector<sf::Vector2f> zSpawns;
		fw.LoadSpawnPoints("data/zombiespawns.txt", zSpawns);
		std::vector<bool> zSpawnsUsed;
		for (int i = 0; i < zSpawns.size(); ++i)
		{
			zSpawnsUsed.push_back(false);
		}

		for (int i = 0; i < zCount; ++i)
		{
			//std::cout << zPosTmp.x << ":" << zPosTmp.y << std::endl;
			int r;
			do
			{
				r = rand() % zSpawns.size();
				std::cout << i << " > " << r << std::endl;
			} while (zSpawnsUsed.at(r));
			zSpawnsUsed.at(r) = true;
			z.setPosition(zSpawns.at(i));
			zombies.push_back(z);
		}
	}
	//*** cz

	//SCP049
	SCP049 plagueDoctor;
	if (!plagueDoctor.setSprite("tex/SCP049.png"))
	{
		std::cout << "Couldn't load \"tex / SCP049.png\"." << std::endl;
		return 1;
	}
	plagueDoctor.setScale(0.08f);
	plagueDoctor.setPosition(3010.0f, 2150.0f);
	plagueDoctor.enable(false);

	for (int i = 0; i < 7; ++i)
	{
		plagueDoctor.talk[i].setBuffer(soundbuffer[i + 8]);
		plagueDoctor.talk[i].setRelativeToListener(true);
		plagueDoctor.talk[i].setPosition(0, 0, 0);
		plagueDoctor.talk[i].setVolume(80);
	}

	plagueDoctor.knife.setBuffer(soundbuffer[16]);

	float plagueDoctorDelay = 30.0f;
	//*** scp

	sf::Event eve;

	bool quit = false;


	sf::Clock clock;
	float dt = 0.016f;

	int cnt = 0;

	sf::View view = window.getDefaultView();
	float zoomFactor = 1.0f;
	view.zoom(0.5f);
	view.zoom(1.1f);
	bool show_fps = false;

	window.setVerticalSyncEnabled(false);

	fw.LoadLightShapesFromFile("data/Level1_3_big.jpg.txt", lightShapes, ls);
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
				/*
				if (eve.mouseWheelScroll.delta < 0 && zoomFactor < 2.0f)
				{
					zoomFactor += 0.1f;
					view.zoom(1.05f);
				}
				else if (eve.mouseWheelScroll.delta > 0 && zoomFactor > 0.5f)
				{
					zoomFactor -= 0.1f;
					view.zoom(0.95f);
				}*/
				break;
			}

			if (eve.type == sf::Event::MouseButtonPressed)
			{
				if (eve.mouseButton.button == sf::Mouse::Left && state == Running)
				{
					if (mainChar.getFlashlightSwitch() == false)
						flashlightSnd[0].play();
					else
						flashlightSnd[1].play();

					mainChar.toogleFlashlight();
				}
				else if (eve.mouseButton.button == sf::Mouse::Right && state == Running)
				{
					bool collided = false;
					sf::FloatRect range;
					range.height = range.width = 100.0f;
					range.left = mainChar.getPosition().x - 50.0f;
					range.top = mainChar.getPosition().y - 50.0f;
					for (int i = 0; i < zombies.size() && !collided; ++i)
					{
						if(range.intersects(zombies.at(i).getBoundingBox()))
						{
							std::cout << "Attack!\n";
							mainChar.attack(zombies.at(i));
							if (zombies.at(i).getState() == Character::Dead)
							{
								int r = rand() % 2;
								zombies.at(i).setSprite(ZombieDeadTex[r]);
								std::cout << "Zombie died or is dead\n";
							}
							break;
						}
					}
					collided = false;
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
					//quit = true;
					if (state == Running)
						state = Paused;
					else if (state == Paused || state == Win || state == Lose)
						quit = true;
					break;

				case sf::Keyboard::Space:
					if (state == Paused)
						state = Running;
					break;

				case sf::Keyboard::Return:
					if (state == Intro)
					{
						state = Running;
						music.stop();
						int r = rand() % 7;
						plagueDoctor.talk[r].play();
					}
					break;
				}
			}
		}

		if (mainChar.getState() != Character::Dead && state == Running)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				mainChar.setState(Character::Run);
			else
				mainChar.setState(Character::Walk);

			//if (mainChar.getFlashlightSwitch())
				//lightPoint->_emissionSprite.setColor(sf::Color::Black);
			//else
			lightPoint->_emissionSprite.setColor(pointColor);
			lightPoint->_emissionSprite.setPosition(mainChar.getPosition());

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
			sf::Vector2f mainCharOldPos(mainChar.getPosition());
			mainChar.move(moveVec, dt);

			if (mainCharOldPos != mainChar.getPosition())
			{
				footstepCnt += dt;

				float thres;

				if (mainChar.getState() == Character::Walk)
					thres = 0.6f;
				else
					thres = 0.4f;

				if (footstepCnt > thres)
				{
					footstepCnt = 0;
					footstepSnd[rand() % 4].play();
				}
			}

			sf::FloatRect bgBounds = bgSprite.getGlobalBounds();
			float border = 30.0f;
			bgBounds.height -= border;
			bgBounds.width -= border;
			bgBounds.left += border;
			bgBounds.top += border;
			mainChar.update(dt);
			if (!bgBounds.contains(mainChar.getPosition()))
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
					if (zombies.at(i).getState() != Character::Dead && mainChar.collide(zombies.at(i)))
						mainChar.move(-moveVec, dt);
				}
			}

			if (mainChar.getSprite().getGlobalBounds().intersects(Key.getGlobalBounds()))
			{
				mainChar.collectKey();
				Key.setScale(0.5f, 0.5f);
				Key.setPosition(window.getDefaultView().getSize().x - 200.0f, 50.0f);
			}
			//*** mccd

			sf::Listener::setPosition(mainChar.getPosition().x, mainChar.getPosition().y, 0);

			//Zombie AI
			collided = false;
			for (int i = 0; i < zombies.size(); ++i)
			{
				//sf::Vector3f sndTmp(zombies.at(i).getPosition().x, zombies.at(i).getPosition().y, 0);
				 //zombies.at(i).breath[0].setPosition(sndTmp);
				if (zombies.at(i).getState() != Character::Dead)
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
						zombies.at(i).attack(mainChar);
						zombies.at(i).move(-zmov, dt);
					}
				}
			}
			//*** zai

			//PlagueDoctor AI
			if (plagueDoctorDelay > 0.0f && !plagueDoctor.isEnabled())
			{
				plagueDoctorDelay -= dt;
			}
			
			if (plagueDoctorDelay < 0.0f && !plagueDoctor.isEnabled())
			{
				plagueDoctorDelay = 1.0f;
				plagueDoctor.enable(true);
				plagueDoctor.knife.play();
			}

			sf::Vector2f pdmov = plagueDoctor.think(mainChar) * dt * plagueDoctor.getWalkSpeed();
			plagueDoctor.move(pdmov, dt);

			if (plagueDoctor.isEnabled() && mainChar.getSprite().getGlobalBounds().intersects(plagueDoctor.getBoundingBox()))
			{
				plagueDoctor.attack(mainChar);
				plagueDoctor.knife.stop();
			}

			plagueDoctor.update(dt);
			//*** pdai

			view.setCenter(mainChar.getPosition());
			healthBar.setString(std::to_string(static_cast<int>(mainChar.getHealth())));

			//Flashlight Rotation
			sf::Vector2f v = light->_emissionSprite.getPosition() - mousePos;
			light->_emissionSprite.setRotation((atan2f(v.y, v.x) * 180 / 3.1415f) + 135.0f);
			mainChar.setRotation((atan2f(v.y, v.x) * 180 / 3.1415f) + 90.0f);
			//** fr

			//GameState
			if (state == Running && mainChar.hasKey() && mainChar.getSprite().getGlobalBounds().intersects(exit))
			{
				state = Win;
				mainChar.setState(Character::Dead);
			}

			if (mainChar.getState() == Character::Dead && state != Win)
			{
				state = Lose;
			}
			//*** gs
		}

		window.clear();

		if (state == Running || state == Paused)
		{
			window.setView(view);

			window.draw(bgSprite);

			if (!mainChar.hasKey())
				window.draw(Key);

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

			if(plagueDoctor.isEnabled())
				window.draw(plagueDoctor.getSprite());

			sf::RectangleShape rangedraw(sf::Vector2f(100.0f, 100.0f));
			rangedraw.setPosition(mainChar.getPosition().x - 50.0f, mainChar.getPosition().y - 50.0f);
			//window.draw(rangedraw);

			window.draw(bgTopSprite);

			ls.render(view, unshadowShader, lightOverShapeShader);
			sf::Sprite lightSprite;
			lightSprite.setTexture(ls.getLightingTexture());

			sf::RenderStates lightRenderStates;
			
			sf::RenderTexture lightMultiLayer;
			lightMultiLayer.create(lightSprite.getTextureRect().width, lightSprite.getTextureRect().height);
			lightRenderStates.blendMode = sf::BlendAdd;

			bgLightSprite.setScale(1, -1);
			//bgLightSprite.setPosition(0, 0);
			lightSprite.setScale(1, -1);
			lightSprite.setPosition(0, static_cast<float>(vm.height));

			lightMultiLayer.draw(lightSprite, lightRenderStates);
			sf::View view2(view);
			view2.setCenter(view2.getCenter().x, -view2.getCenter().y);
			lightMultiLayer.setView(view2);
			lightMultiLayer.draw(bgLightSprite, lightRenderStates);

			lightRenderStates.blendMode = sf::BlendMultiply;

			//lightSprite.setTexture(lightMultiLayer.getTexture());
			sf::Sprite lightMultiSprite;
			lightMultiSprite.setTexture(lightMultiLayer.getTexture());

			window.setView(window.getDefaultView());
			window.draw(lightMultiSprite, lightRenderStates);
			
			lightRenderStates.blendMode = sf::BlendMultiply;

			window.setView(window.getDefaultView());

			sf::RectangleShape bloodyScreen(sf::Vector2f(vm.width, vm.height));
			bloodyScreen.setFillColor(sf::Color(255, (1.0f - (mainChar.gotHit() / 0.15f)) * 255, (1.0f - (mainChar.gotHit() / 0.15f)) * 255));
			if (mainChar.gotHit() > 0)
			{
				//std::cout << (mainChar.gotHit()/0.3f) * 255 << std::endl;
				window.draw(bloodyScreen, lightRenderStates);
			}

			window.draw(healthBar);

			if (mainChar.hasKey())
				window.draw(Key);

			if (state == Paused)
			{
				bloodyScreen.setFillColor(sf::Color(0, 0, 0, 128));
				window.draw(bloodyScreen);
				loading.setPosition(50.0f, 50.0f);
				loading.setString("paused...");
				window.draw(loading);
			}

			window.setView(view);
		}
		else if (state == Win || state == Lose)
		{
			if(music.getStatus() != sf::SoundSource::Playing)
				music.play();

			if(died == false)
			{
				died = true;
				if(state == Lose)
					kill.play();
			}

			if(state == Win)
				endTitle.setString("YOU ESCAPED");
			else
			{
				endTitle.setString("YOU DIED");
			}

			window.setView(window.getDefaultView());
			endTitle.setPosition((vm.width - endTitle.getGlobalBounds().width) / 2, (vm.height - endTitle.getGlobalBounds().height)/2);
			window.draw(endTitle);

			loading.setString("please press ESC to exit...");
			loading.setPosition((vm.width - loading.getGlobalBounds().width) / 2, ((vm.height - loading.getGlobalBounds().height) / 2) + 120);
			window.draw(loading);
		}
		else if(state == Intro)
		{
			loading.setString(introText);
			//loading.setString("press ENTER to start");
			loading.setPosition((vm.width - loading.getGlobalBounds().width) / 2, (vm.height - loading.getGlobalBounds().height) / 2);
			window.draw(loading);
		}

		window.display();

		dt = clock.getElapsedTime().asSeconds();

		cnt++;
		if (cnt % 100 == 0 && show_fps)
		{
			std::cout << dt << " - " << (1.0f / dt) << " - " << mainChar.getRotation() << std::endl;
			
			//sf::Vector2f tf = view.getCenter() - (view.getSize()/2.0f);
			//std::cout << " - " << tf.x << "/" << tf.y << "/" << view.getSize().x << "/" << view.getSize().y << std::endl;
			cnt = 0;
		}
	}

	for (auto& t : threads)
		t.join();

	window.close();
	return 0;
}

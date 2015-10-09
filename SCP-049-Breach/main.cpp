#include <SFML/Graphics.hpp>
#include <iostream>

#include "Map.h"

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SCP-049-Breach");

	Map main;
	main.LoadFromFile("./tex/testMap.png", "./tex/testMap_c.png");

	sf::Texture texture;
	if (!texture.loadFromFile("./tex/Sprite_01.png"))
	{
		cout << "Failed to load Texture" << endl;
	}

	sf::Sprite testSprite;
	testSprite.setTexture(texture);

	window.draw(testSprite);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(main.getDiffuseSprite());
		window.draw(testSprite);
		window.display();
	}

	return 0;
}

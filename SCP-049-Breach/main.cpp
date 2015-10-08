#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(400, 400), "SCP-049-Breach");

	sf::Texture texture;
	if (!texture.loadFromFile("C:/FH/Sem5/GameDev/SCP-049-Breach/x64/Debug/tex/Sprite_01.png"))
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
		window.draw(testSprite);
		window.display();
	}

	return 0;
}

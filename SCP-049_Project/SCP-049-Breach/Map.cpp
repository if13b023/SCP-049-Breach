#include "Map.h"

Map::Map()
{
}


Map::~Map()
{
}

int Map::LoadFromFile(char* pathDiff, char* pathColl)
{
	if (!diffuseMap.loadFromFile(pathDiff))
		return 1;//Error
	collisionMap.loadFromFile(pathColl);
	//if (!collisionMap.loadFromFile(pathColl))
	//	return 1;//Error
	
	diffuseSprite.setTexture(diffuseMap);

	return 0; //Everything OK
}

sf::Sprite Map::getDiffuseSprite() {
	return diffuseSprite;
}

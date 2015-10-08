#include "Map.h"

Map::Map()
{
}


Map::~Map()
{
}

int Map::LoadFromFile(std::string path)
{
	if (!diffuseMap.loadFromFile(path))
		return 1;//Error
	if (!collisionMap.loadFromFile(path.append("_c")))
		return 1;//Error
	
	diffuseSprite.setTexture(diffuseMap);
	collisionSprite.setTexture(collisionMap);

	return 0; //Everything OK
}

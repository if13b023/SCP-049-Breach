#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

#include "SFML/Graphics.hpp"
#include "ltbl\lighting\LightShape.h" 
#include "ltbl\lighting\LightSystem.h"

class FileWriter
{
public:
	FileWriter();
	~FileWriter();
	
	void SaveLightShapes(std::string, std::vector<std::shared_ptr<ltbl::LightShape>>&);
	void LoadLightShapesFromFile(std::string, std::vector<std::shared_ptr<ltbl::LightShape>>&, ltbl::LightSystem&);
	
	void LoadSpawnPoints(const char* path, std::vector<sf::Vector2f>& spawns);
};


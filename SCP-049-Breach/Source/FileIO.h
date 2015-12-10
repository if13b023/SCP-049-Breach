#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

#include "ltbl\lighting\LightShape.h" 
#include "ltbl\lighting\LightSystem.h"

class FileWriter
{
public:
	FileWriter();
	~FileWriter();
	
	void SaveLightShapes(std::string, std::vector<std::shared_ptr<ltbl::LightShape>>&);
	void LoadLightShapesFromFile(std::string, std::vector<std::shared_ptr<ltbl::LightShape>>&, ltbl::LightSystem&);
};


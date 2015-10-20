#include "FileIO.h"

FileWriter::FileWriter()
{
}


FileWriter::~FileWriter()
{
}

void FileWriter::SaveLightShapes(std::string path, std::map<int, std::shared_ptr<ltbl::LightShape>>& list)
{
	std::ofstream file;
	file.open(path);

	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}
	
	file << "start\n";
	sf::ConvexShape ls;
	//stackoverflow
	for (std::map<int, std::shared_ptr<ltbl::LightShape>>::iterator it = list.begin(); it != list.end(); ++it)
	{
		ls = it->second.get()->_shape;
		file << "id:" << it->first << std::endl;
		file << ls.getPosition().x << ":" << ls.getPosition().y << std::endl;
		file << ls.getPointCount() << std::endl;
		for (int i = 0; i < ls.getPointCount(); i++)
		{
			file << ls.getPoint(i).x << ":" << ls.getPoint(i).y << std::endl;
		}

	}
	file << "end\n";
	file.close();
	/*
	for (std::map<int, std::shared_ptr<ltbl::LightShape>>::iterator it = list.begin(); it != list.end(); ++it)
	{
		ls = it->second.get()->_shape;
		myfile << "id:" << it->first << std::endl;
		myfile << ls.getOrigin().x << ":" << ls.getOrigin().y << std::endl;
		myfile << ls.getPointCount() << std::endl;
		for (int i = 0; i < ls.getPointCount(); i++)
		{
			myfile << ls.getPoint(i).x << ":" << ls.getPoint(i).y << std::endl;
		}

	}*/
}

void FileWriter::LoadLightShapesFromFile(std::string path, std::map<int, std::shared_ptr<ltbl::LightShape>>& list, ltbl::LightSystem& ls)
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	//Warning C4267: http://www.cplusplus.com/reference/map/map/size/ -> Member type size_type is an unsigned integral type. Ok?
	//unsigned int Index = list.size();
	int index = 0,
		pnt_cnt = 0;
	std::string tmp = "";

	do
	{
		std::getline(file, tmp);
		//std::cout << tmp << std::endl;
		std::cout << "nothing found...\n";
	} while (tmp.compare("start") != 0 && !file.eof());

	if (tmp.compare("start") == 0)
	{
		list.clear();
		std::cout << "start found!\n";
	}
	else
	{
		std::cout << "something went wrong\n";
		return;
	}

	std::getline(file, tmp, '\n');
	int init = 0;

	do
	{
		int pos = tmp.find(':');
		if (tmp.substr(0, pos).compare("id") == 0)
		{
			if(init > 0)
				ls.addShape(list[index]);

			index++;
			pnt_cnt = 0;
			std::cout << "*** new light shape ***\nid = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list[index] = std::make_shared<ltbl::LightShape>();
			std::getline(file, tmp, '\n');
			pos = tmp.find(':');
			std::cout << "origin: x = " << tmp.substr(0, pos) << "\ty = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list[index]->_shape.setPosition( std::stof(tmp.substr(0, pos)), std::stof(tmp.substr(pos + 1, tmp.length() - pos)));
			std::getline(file, tmp, '\n');
			std::cout << "# of point = " << tmp << std::endl;
			list[index]->_shape.setPointCount(std::stoi(tmp));
		}
		else
		{
			std::cout << "x = " << tmp.substr(0, pos) << "\ty = " << tmp.substr(pos + 1, tmp.length() - pos) << std::endl;
			list[index]->_shape.setPoint(pnt_cnt, sf::Vector2f(std::stof(tmp.substr(0, pos)), std::stof(tmp.substr(pos + 1, tmp.length() - pos))));
			pnt_cnt++;
		}
		std::getline(file, tmp, '\n');
		init = 1;
	} while (tmp.compare("end") != 0 && !file.bad() && !file.eof());

	ls.addShape(list[index]);
}

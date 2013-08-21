#include "ResourceManager.h"
#include "Parser.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace Feanwork
{
	void ResourceManager::expandBatch(std::string _dir, std::string _batchFile)
	{
		if(_dir != "")
		{
			std::string path = _dir;
			if((path.back() != '\\') && (path.back() != '/'))
				path += "/*.png";
			else
				path += "*.png";

			WIN32_FIND_DATA info;
			HANDLE searchHandle = FindFirstFile(path.c_str(), &info);
			std::vector<std::string> stringList;

			if(searchHandle == INVALID_HANDLE_VALUE)
			{
				std::cerr << "Could not find images, this could be because the directory does not contain images" <<
				"or the first file is not an image\n";
			}

			do stringList.push_back(info.cFileName);
			while(FindNextFile(searchHandle, &info));
			FindClose(searchHandle);

			std::ofstream oStream(_batchFile, std::fstream::app);
			std::ifstream iStream(_batchFile);
			if(!oStream.is_open() || !iStream.is_open())
			{
				std::cout << "Could not locate batch file\n";
				return;
			}

			std::vector<std::string> added;
			while(!iStream.eof())
			{
				std::string string;
				std::getline(iStream, string);

				if(!string.empty() && (string.find_first_of("~") == std::string::npos))
					added.push_back(string);
			}
			iStream.close();

			bool unchanged = true;
			for(unsigned i = 0; i < stringList.size(); i++)
			{
				std::string newLine;
				std::string texture;

				std::stringstream sStream;
				sStream << added.size();

				texture  = _dir + "/" + stringList[i];
				newLine  = "#Resource ";
				newLine += sStream.str();
				newLine += ", "; 
				newLine += texture;

				bool compares = false;
				for(unsigned t = 0; t < added.size(); t++)
				{
					std::string strCatch = "";
					int start = added[t].find_first_of("/");
					start++;
					strCatch = added[t].substr(start);

					if(strCatch == stringList[i])
						compares = true;
				}

				if(compares)
					continue;

				oStream << newLine.c_str() << std::endl;
				added.push_back(newLine);

				if(unchanged)
					unchanged = false;
			}

			if(!unchanged)
				oStream << std::endl;

			oStream.close();
		}
	}

	void ResourceManager::loadBatch(std::string _dir, std::string _batchFile)
	{
		std::string keyword = "";
		Parser p(_dir + _batchFile);

		while((keyword = p.getNextKeyword()) != "")
			if(keyword == "Resource")
			{
				if(!p.checkNumParams(2))
					std::cout << "Incorrect number of parameters passed to resource" << endl;

				sf::Texture* newTex = new sf::Texture;
				newTex->loadFromFile(_dir + p.getString(1));
				mResources[p.getInt(0)] = newTex;
			}
	}

	sf::Texture* ResourceManager::getResource(int _id)
	{
		std::map<int, sf::Texture*>::iterator it;
		for(it = mResources.begin(); it != mResources.end(); ++it)
			if(it->first == _id)
				return it->second;

		return NULL;
	}
}
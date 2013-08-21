#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

namespace Feanwork
{
#define RESOURCE_MAP std::map<int, sf::Texture*>

	class ResourceManager
	{
	public:
		void expandBatch(std::string _globalDir, std::string _dir, std::string _batchFile);
		void loadBatch(std::string _dir, std::string _batchFile);
		sf::Texture* getResource(int _id);

		static ResourceManager* getSingleton()
		{
			static ResourceManager singleton;
			return &singleton;
		}

	protected:
		ResourceManager() {}
		virtual ~ResourceManager() {}

		RESOURCE_MAP mResources;
	};
}

#endif

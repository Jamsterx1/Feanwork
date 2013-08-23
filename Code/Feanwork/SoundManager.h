#ifndef SOUNDQUEUE_H_
#define SOUNDQUEUE_H_

#include <SFML/Audio.hpp>
#include <vector>
#include <map>

namespace Feanwork
{
	#define SOUNDMAP		  std::map<std::string, sf::Sound*>
	#define SOUNDMAP_ITERATOR std::map<std::string, sf::Sound*>::iterator

	class SoundManager
	{
	public:
		SoundManager() {}
		~SoundManager() {}

		bool	   update();
		void	   addSound(std::string _name, std::string _sound);
		sf::Sound* getSound(std::string _name);
		void	   playSound(std::string _sound);

	protected:
		std::vector<sf::Sound*> mSoundQueue;
		SOUNDMAP				 mSounds;
	};
}

#endif

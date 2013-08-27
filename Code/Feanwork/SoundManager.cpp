#include "SoundManager.h"

namespace Feanwork
{
	bool SoundManager::update()
	{
		unsigned size = mSoundQueue.size();
		for(unsigned i = 0; i < size; i++)
			if(mSoundQueue[i]->getStatus() == sf::SoundSource::Status::Stopped)
			{
				delete mSoundQueue[i];
				mSoundQueue.erase(mSoundQueue.begin() + i);
				size--;
			}

		return true;
	}

	void SoundManager::addSound(std::string _name, std::string _sound)
	{
		sf::SoundBuffer* buffer = new sf::SoundBuffer;
		buffer->loadFromFile(_sound);

		sf::Sound* sound = new sf::Sound;
		sound->setBuffer(*buffer);
		sound->setVolume(50.f);
		mSounds[_name] = sound;
	}

	sf::Sound* SoundManager::getSound(std::string _name)
	{
		return mSounds[_name];
	}

	void SoundManager::playSound(std::string _name)
	{
		sf::Sound* sound = new sf::Sound(*getSound(_name));
		sound->play();
		mSoundQueue.push_back(sound);
	}
}

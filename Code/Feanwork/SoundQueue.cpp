#include "SoundQueue.h"

namespace Feanwork
{
	bool SoundQueue::update()
	{
		for(unsigned i = 0; i < mSoundQueue.size(); i++)
			if(mSoundQueue[i].getStatus() == sf::SoundSource::Status::Stopped)
				mSoundQueue.erase(mSoundQueue.begin() + i);

		return true;
	}

	void SoundQueue::pushSound(std::string _sound)
	{
		sf::SoundBuffer* buffer = new sf::SoundBuffer;
		buffer->loadFromFile(_sound);

		sf::Sound sound;
		sound.setBuffer(*buffer);
		sound.play();
		mSoundQueue.push_back(sound);
	}
}

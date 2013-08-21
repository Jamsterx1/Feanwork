#ifndef SOUNDQUEUE_H_
#define SOUNDQUEUE_H_

#include <SFML/Audio.hpp>
#include <vector>

namespace Feanwork
{
	class SoundQueue
	{
	public:
		SoundQueue() {}
		~SoundQueue() {}

		bool update();
		void pushSound(std::string _sound);

	protected:
		std::vector<sf::Sound> mSoundQueue;
	};
}

#endif

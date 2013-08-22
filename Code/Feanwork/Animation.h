#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <SFML/Graphics.hpp>
#include <vector>

namespace Feanwork
{
	struct Frame
	{
		int x;
		int y;
		int width;
		int height;

		int positionID;
	};

	#define FILE std::string
	#define ANIMATION std::vector<Frame>

	class Game;
	class Object;
	class Animation
	{
	public:
		Animation(Object* _hook, FILE _animSet);
		~Animation();

		bool		 update(Game* _game);
		const Frame* nextFrame(FILE _anim, int _frame);
		void		 changeAnim(FILE _anim);

	protected:
		std::map<std::string, ANIMATION> mAnimations;
		Object*							 mHook;
		int								 mStepCount;

		std::string	mCurrentAnim;
		int			mCurrentFrame;
		bool		mFlipped;
	};
}

#endif
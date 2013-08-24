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
		Animation(Object* _hook, FILE _animSet, float _frameSpeed = 0.5f, bool _loop = true);
		~Animation();

		bool		 update(Game* _game);
		const Frame* nextFrame(FILE _anim, int _frame);
		void		 changeAnim(FILE _anim);
		void		 setFrameSpeed(float _speed) { mFrameSpeed = _speed; }

	protected:
		std::map<std::string, ANIMATION> mAnimations;
		Object*							 mHook;
		float							 mStepCount;
		float							 mFrameSpeed;

		std::string	mCurrentAnim;
		int			mCurrentFrame;
		bool		mFlipped;
		bool		mLoop;
	};
}

#endif
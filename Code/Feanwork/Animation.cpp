#include "Animation.h"
#include "Object.h"
#include "Game.h"
#include "Parser.h"

namespace Feanwork
{
	Animation::Animation(Object* _hook, FILE _animSet, bool _loop)
	{
		mHook	   = _hook;
		mStepCount = 0;
		mLoop	   = _loop;

		std::string keyword = "";
		Parser p(_animSet);

		while((keyword = p.getNextKeyword()) != "")
		{
			if(keyword == "Frame")
			{
				if(!p.checkNumParams(5))
					cout << "Incorrect number of parameters passed to frame" << endl;

				std::string anim = p.getString(0);
				int frameData[4] = { p.getInt(1), p.getInt(2), p.getInt(3), p.getInt(4) };
				int id = 0;
				Frame* frame = new Frame;

				if(!mAnimations[anim].empty())
					id = mAnimations[anim].back().positionID + 1;

				frame->x		  = frameData[0];
				frame->y		  = frameData[1];
				frame->width	  = frameData[2];
				frame->height	  = frameData[3];
				frame->positionID = id;
				mAnimations[anim].push_back(*frame);
			}
			else
				std::cout << "Keyword: '" << keyword << "' is unknown\n";
		}

		mCurrentAnim  = "walk";
		mCurrentFrame = 0;
		mFlipped	  = false;

		ANIMATION& anim = mAnimations[mCurrentAnim];
		mHook->setRect(anim[0]);
	}
		
	Animation::~Animation()
	{
	}

	bool Animation::update(Game* _game)
	{
		if(mLoop && mStepCount >= 5)
		{
			mHook->setRect(*nextFrame(mCurrentAnim, mCurrentFrame));
			mStepCount = 0;
		}
		else if(!mLoop && mStepCount >= 5)
		{
			mStepCount = 0;
			if(mCurrentFrame < (int)mAnimations[mCurrentAnim].size() - 1)
				mHook->setRect(*nextFrame(mCurrentAnim, mCurrentFrame));
		}
		else
			mStepCount++;

		return true;
	}

	const Frame* Animation::nextFrame(std::string _anim, int _frame)
	{
		ANIMATION& anim = mAnimations[mCurrentAnim];
		for(unsigned i = 0; i < anim.size(); i++)
			if(anim[i].positionID == anim[mCurrentFrame].positionID)
			{
				if((i + 1) <= anim.size() - 1)
				{
					mCurrentFrame = i + 1;
					return &anim[mCurrentFrame];
				}
				else
				{
					mCurrentFrame = 0;
					return &anim[mCurrentFrame];
				}
			}

		return nullptr;
	}

	void Animation::changeAnim(std::string _anim)
	{
		if(mCurrentAnim == _anim)
			return;

		mCurrentAnim  = _anim;
		mCurrentFrame = 0;
	}
}

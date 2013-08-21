#include "Switch.h"
#include "InterfaceManager.h"
#include "../ResourceManager.h"
#include "../EventManager.h"
#include "../Game.h"

namespace Feanwork
{
	Switch::Switch(INTERFACEBLOCK* _parentBlock, sf::Vector2f _position, std::string _name) :
		Interface(_position, sf::Vector2f(0.0f, 0.0f), _name)
	{
		mParentBlock = _parentBlock;
		mSwitchState = SWITCHSTATE_INACTIVE;
		mClicked	 = false;

		mRecent		 = false;
		mCooldown	 = 0;

		setPosition(mParentBlock->getPosition() + _position);
	}

	Switch::~Switch()
	{
	}

	void Switch::initialize(int _resourceIDs[2], UICallback _callback, void* _data)
	{
		mData.textures[SWITCHSTATE_INACTIVE] = ResourceManager::getSingleton()->getResource(_resourceIDs[SWITCHSTATE_INACTIVE]);
		mData.textures[SWITCHSTATE_ACTIVE] = ResourceManager::getSingleton()->getResource(_resourceIDs[SWITCHSTATE_ACTIVE]);

		mData.sprites[SWITCHSTATE_INACTIVE].setTexture(*mData.textures[SWITCHSTATE_INACTIVE]);
		mData.sprites[SWITCHSTATE_ACTIVE].setTexture(*mData.textures[SWITCHSTATE_ACTIVE]);
		mData.sprites[SWITCHSTATE_INACTIVE].setPosition(mPosition.x, mPosition.y);
		mData.sprites[SWITCHSTATE_ACTIVE].setPosition(mPosition.x, mPosition.y);

		mCallback		= _callback;
		mCallbackData	= _data;
		setBounds((float)mData.textures[0]->getSize().x, (float)mData.textures[0]->getSize().y);
	}

	bool Switch::update(Game*_game)
	{
		if(!mActive)
			return true;

		mInside = false;
		EventManager* manager = EventManager::getSingleton();
		sf::Vector2i mouse = manager->getMousePos(_game);
	
		if(mouse.x > mPosition.x && mouse.x < (mPosition.x + mBounds.x) && mouse.y > mPosition.y && mouse.y < (mPosition.y + mBounds.y))
			mInside = true;

		if(mRecent && mCooldown > 5)
			mRecent = false;

		mClicked = false;
		if(!mRecent && pressed())
		{
			if(mSwitchState == SWITCHSTATE_INACTIVE)
			{
				mSwitchState = SWITCHSTATE_ACTIVE;
				callEvent();
			}
			else if(mSwitchState == SWITCHSTATE_ACTIVE)
			{
				mSwitchState = SWITCHSTATE_INACTIVE;
				callEvent();
			}

			mRecent	  = true;
			mClicked  = true;
			mCooldown = 0;
		}
		else
			mCooldown++;

		return true;
	}

	bool Switch::draw(Game*_game)
	{
		if(mData.textures[mSwitchState])
			_game->getWindow()->draw(mData.sprites[mSwitchState]);

		return true;
	}

	bool Switch::pressed()
	{
		if(mInside && EventManager::getSingleton()->mousePressedOnFrame("left"))
			return true;

		return false;
	}

	void Switch::setSwitchState(SWITCHSTATE _state)
	{
		mSwitchState = _state;
	}

	void Switch::callEvent()
	{
		if(mCallback)
			mCallback(mCallbackData);
	}

	bool Switch::isActive()
	{
		return (mSwitchState == SWITCHSTATE_ACTIVE);
	}
}

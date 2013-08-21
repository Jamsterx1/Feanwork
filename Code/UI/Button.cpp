#include "../Game.h"
#include "../ResourceManager.h"
#include "../EventManager.h"
#include "InterfaceManager.h"

namespace Feanwork
{
	Button::Button(INTERFACEBLOCK* _parentBlock,  sf::Vector2f _position, std::string _name) :
		Interface(_position, sf::Vector2f(0.0f, 0.0f), _name)
	{
		mParentBlock = _parentBlock;
		mClickState  = CLICKSTATE_Released;
		mClicked	 = false;

		setPosition(mParentBlock->getPosition() + _position);
	}

	Button::~Button()
	{
	}

	void Button::initialize(int _resourceIDS[3], UICallback _callback, void* _data)
	{
		mData.textures[CLICKSTATE_Released] = ResourceManager::getSingleton()->getResource(_resourceIDS[CLICKSTATE_Released]);
		mData.textures[CLICKSTATE_Hover]    = ResourceManager::getSingleton()->getResource(_resourceIDS[CLICKSTATE_Hover]);
		mData.textures[CLICKSTATE_Pressed]  = ResourceManager::getSingleton()->getResource(_resourceIDS[CLICKSTATE_Pressed]);

		mData.sprites[CLICKSTATE_Released].setTexture(*mData.textures[CLICKSTATE_Released]);
		mData.sprites[CLICKSTATE_Hover].setTexture(*mData.textures[CLICKSTATE_Hover]);
		mData.sprites[CLICKSTATE_Pressed].setTexture(*mData.textures[CLICKSTATE_Pressed]);
	
		mData.sprites[CLICKSTATE_Released].setPosition(mPosition.x, mPosition.y);
		mData.sprites[CLICKSTATE_Hover].setPosition(mPosition.x, mPosition.y);
		mData.sprites[CLICKSTATE_Pressed].setPosition(mPosition.x, mPosition.y);

		mCallback		= _callback;
		mCallbackData	= _data;
		setBounds((float)mData.textures[0]->getSize().x, (float)mData.textures[0]->getSize().y);
	}

	bool Button::update(Game* _game)
	{
		if(!mActive)
			return true;

		mInside = false;
		EventManager* manager = EventManager::getSingleton();
		sf::Vector2i mouse = manager->getMousePos(_game);
	
		if(mouse.x > mPosition.x && mouse.x < (mPosition.x + mBounds.x) && mouse.y > mPosition.y && mouse.y < (mPosition.y + mBounds.y))
			mInside = true;

		if(!mClicked && mInside)
			mClickState = CLICKSTATE_Hover;
		else if(!mClicked && !mInside)
			mClickState = CLICKSTATE_Released;

		mClicked = false;
		if(pressed())
		{
			mClickState = CLICKSTATE_Pressed;
			callEvent();
			mClicked = true;
		}

		return true;
	}

	bool Button::draw(Game* _game)
	{
		if(mData.textures[mClickState])
			_game->getWindow()->draw(mData.sprites[mClickState]);

		return true;
	}

	bool Button::pressed()
	{
		if(mInside && EventManager::getSingleton()->mousePressedOnFrame("left"))
			return true;

		return false;
	}

	void Button::callEvent()
	{
		if(mCallback)
			mCallback(mCallbackData);
	}
}

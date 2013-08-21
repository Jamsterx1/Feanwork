#include "Interface.h"
#include "../EventManager.h"
#include "../Game.h"

namespace Feanwork
{
	Interface::Interface(sf::Vector2f _position, sf::Vector2f _bounds, std::string _name)
	{
		mPosition	= _position;
		mBounds		= _bounds;
		mInside		= false;
		mActive		= true;
		mUniqueName = _name;
	}

	Interface::~Interface()
	{
	}

	bool Interface::update(Game*_game)
	{
		if(!mActive)
			return true;

		mInside = false;
		EventManager* manager = EventManager::getSingleton();
		sf::Vector2i mouse = manager->getMousePos(_game);
	
		if(mouse.x > mPosition.x && mouse.x < (mPosition.x + mBounds.x) && mouse.y > mPosition.y && mouse.y < (mPosition.y + mBounds.y))
			mInside = true;

		return true;
	}

	void Interface::setPosition(float _x, float _y)
	{
		mPosition.x = _x;
		mPosition.y = _y;
	}

	void Interface::setPosition(sf::Vector2f _position)
	{
		mPosition = _position;
	}

	void Interface::setBounds(float _x, float _y)
	{
		mBounds.x = _x;
		mBounds.y = _y;
	}

	void Interface::setBounds(sf::Vector2f _bounds)
	{
		mBounds = _bounds;
	}
}

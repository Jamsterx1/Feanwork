#include "Object.h"
#include "Game.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

namespace Feanwork
{
	Object::Object(int _resourceID, float _xPos, float _yPos, bool _canCollide, bool _active)
	{
		setPosition(_xPos, _yPos);
		mCanCollide = _canCollide;
		mDestroy    = false;
		mActive		= _active;

		sf::Texture* tex = ResourceManager::getSingleton()->getResource(_resourceID);
		mSprite.setTexture(*tex);

		mAABB.left	 = (int)mX;
		mAABB.top	 = (int)mY;
		mAABB.width  = (int)tex->getSize().x / 2;
		mAABB.height = (int)tex->getSize().y / 2;

		mDebug.setOutlineColor(sf::Color::Red);
		mDebug.setOutlineThickness(2);
		mDebug.setFillColor(sf::Color::Transparent);
		mDebug.setPosition(_xPos, _yPos);
		mDebug.setSize(sf::Vector2f((float)mAABB.width * 2.f, (float)mAABB.height * 2.f));
	}

	bool Object::update(Game* _game)
	{
		mColliding = false;
		return true;
	}

	bool Object::render(Game* _game)
	{
		if(mActive)
		{
			_game->getWindow()->draw(mSprite);
			if(_game->getDebugMode() && mCanCollide)
				_game->getWindow()->draw(mDebug);
		}
		return true;
	}

	void Object::collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Object* _collision, Game* _game)
	{
	}

	void Object::setRect(int _x, int _y, int _width, int _height)
	{
		mSprite.setTextureRect(sf::IntRect(_x, _y, _width, _height));
		mAABB.width  = _width;
		mAABB.height = _height;
		mDebug.setSize(sf::Vector2f((float)_width, (float)_height));
	}

	void Object::setRect(Frame _frame)
	{
		mSprite.setTextureRect(sf::IntRect(_frame.x, _frame.y, _frame.width, _frame.height));
		mAABB.width  = _frame.width  / 2;
		mAABB.height = _frame.height / 2;
		mDebug.setSize(sf::Vector2f((float)_frame.width, (float)_frame.height));
	}

	void Object::switchID(int _resourceID)
	{
		mSprite.setTexture(*ResourceManager::getSingleton()->getResource(_resourceID));
	}

	void Object::setPosition(float _x, float _y)
	{
		mX = _x;
		mY = _y;
		mAABB.left = (int)_x;
		mAABB.top  = (int)_y;
		mSprite.setPosition(_x, _y);
		mDebug.setPosition(_x, _y);
	}

	void Object::addPosition(float _x, float _y)
	{
		mX += _x;
		mY += _y;
		mAABB.left = (int)mX;
		mAABB.top  = (int)mY; 
		mSprite.setPosition(mX, mY);
		mDebug.setPosition(mX, mY);
	}
}

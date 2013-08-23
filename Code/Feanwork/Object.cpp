#include "Object.h"
#include "Game.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

namespace Feanwork
{
	Object::Object(int _resourceID, float _xPos, float _yPos, bool _canCollide)
	{
		setPosition(_xPos, _yPos);
		mCanCollide = _canCollide;
		mDestroy    = false;

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
		mDebug.setSize(sf::Vector2f(mAABB.width * 2, mAABB.height * 2));
	}

	bool Object::update(Game* _game)
	{
		return true;
	}

	bool Object::render(Game* _game)
	{
		_game->getWindow()->draw(mSprite);
		if(_game->getDebugMode())
			_game->getWindow()->draw(mDebug);

		return true;
	}

	void Object::collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Game* _game)
	{
	}

	void Object::setRect(int _x, int _y, int _width, int _height)
	{
		mSprite.setTextureRect(sf::IntRect(_x, _y, _width, _height));
		mAABB.width  = _width;
		mAABB.height = _height;
		mDebug.setSize(sf::Vector2f(_width, _height));
	}

	void Object::setRect(Frame _frame)
	{
		mSprite.setTextureRect(sf::IntRect(_frame.x, _frame.y, _frame.width, _frame.height));
		mAABB.width  = _frame.width  / 2;
		mAABB.height = _frame.height / 2;
		mDebug.setSize(sf::Vector2f(_frame.width, _frame.height));
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

#include "EventManager.h"
#include "player.h"
#include "Bullet.h"

Player::Player(int _resourceID, float _xPos, float _yPos) :
	Feanwork::Object(_resourceID, _xPos, _yPos, false),
	Animation(this, "resources/animation/player.anim")
{
	mDelay = 0;
}

Player::~Player()
{
}

bool Player::update(Feanwork::Game* _game)
{
	// input
	EventManager* evm = EventManager::getSingleton();
	if(evm->keyPressed("a"))
	{
		sf::Vector2f position(-200.f * _game->getDelta(), 0.f);
		addPosition(position.x, position.y);
		_game->moveCamera(position.x, position.y);
	}
	else if(evm->keyPressed("d"))
	{
		sf::Vector2f position(200.f * _game->getDelta(), 0.f);
		addPosition(position.x, position.y);
		_game->moveCamera(position.x, position.y);
	}

	if(evm->keyPressed("w"))
	{
		sf::Vector2f position(0.f, -200.f * _game->getDelta());
		addPosition(position.x, position.y);
		_game->moveCamera(position.x, position.y);
	}
	else if(evm->keyPressed("s"))
	{
		sf::Vector2f position(0.f, 200.f * _game->getDelta());
		addPosition(position.x, position.y);
		_game->moveCamera(position.x, position.y);
	}

	if(!_game->isInterfaceActive())
	{
		if(mDelay++ >= 10 && evm->mousePressed("left"))
		{
			sf::Vector2i mousePosition = evm->getMousePos(_game);
			sf::View view = _game->getCamera();
			sf::Vector2f mousePosTrans = _game->getWindow()->mapPixelToCoords(mousePosition, view);
			sf::Vector2f origin(mX + mAABB.width, mY + mAABB.height);

			sf::Vector2f delta(mousePosTrans.x - origin.x, mousePosTrans.y - origin.y);
			float angle = atan2(delta.y, delta.x);
			float power = 500.f;
			int id		= rand() % 3;

			Bullet* bullet = new Bullet(id, origin.x, origin.y, sf::Vector2f(cos(angle) * power, sin(angle) * power));
			ignore(bullet);
			bullet->setUniqueType("bullet");
			_game->pushObject(bullet);
			mDelay = 0;
		}
	}

	Animation::update(_game);
	return true;
}

bool Player::render(Feanwork::Game* _game)
{
	Feanwork::Object::render(_game);
	return true;
}

void Player::collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Game* _game)
{
	addPosition(_depth.x * _normal.x, _depth.y * _normal.y);
	_game->moveCamera(_depth.x * _normal.x, _depth.y * _normal.y);
}

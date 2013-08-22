#include "EventManager.h"
#include "player.h"
#include "Bullet.h"

Player::Player(int _resourceID, float _xPos, float _yPos) :
	Feanwork::Object(_resourceID, _xPos, _yPos, false)
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
		addPosition(-2.f, 0.f);
	else if(evm->keyPressed("d"))
		addPosition( 2.f, 0.f);

	if(evm->keyPressed("w"))
		addPosition(0.f, -2.f);
	else if(evm->keyPressed("s"))
		addPosition(0.f,  2.f);

	if(!_game->isInterfaceActive())
	{
		if(mDelay++ >= 10 && evm->mousePressed("left"))
		{
			sf::Vector2i mousePosition = evm->getMousePos(_game);
			sf::Vector2f origin(mX + (mAABB.width * 2), mY + (mAABB.height * 2));
			sf::Vector2f delta((float)mousePosition.x - origin.x, (float)mousePosition.y - origin.y);
			float angle = atan2(delta.y, delta.x);
			float power = 5.f;
			int   id = rand() % 3;

			Bullet* bullet = new Bullet(id, mX, mY, sf::Vector2f(cos(angle) * power, sin(angle) * power));
			ignore(bullet);
			bullet->setUniqueType("bullet");
			_game->pushObject(bullet);
			mDelay = 0;
		}
	}

	return true;
}

bool Player::render(Feanwork::Game* _game)
{
	Feanwork::Object::render(_game);
	return true;
}

void Player::collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal)
{
	addPosition(_depth.x * _normal.x, _depth.y * _normal.y);
}

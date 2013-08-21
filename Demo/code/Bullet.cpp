#include "Bullet.h"
#include "Feanwork/Game.h"

Bullet::Bullet(int _resourceID, float _xPos, float _yPos, sf::Vector2f _velocity) :
	Feanwork::Object(_resourceID, _xPos, _yPos, true)
{
	mVelocity = _velocity;
	mCounter = 0;
}

Bullet::~Bullet(void)
{
}

bool Bullet::update(Feanwork::Game* _game)
{
	mCounter++;
	if(mCounter >= 600)
		destroy();

	addPosition(mVelocity.x, mVelocity.y);
	return true;
}

bool Bullet::render(Feanwork::Game* _game)
{
	Object::render(_game);
	return true;
}

void Bullet::collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal)
{
	destroy();
}

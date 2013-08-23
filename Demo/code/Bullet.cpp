#include "Bullet.h"
#include "Emitter.h"

Bullet::Bullet(int _resourceID, float _xPos, float _yPos, sf::Vector2f _velocity) :
	Feanwork::Object(_resourceID, _xPos, _yPos, true),
	mEmitter(sf::Vector2f(mX, mY), sf::Vector2f(0.f, 0.f), Feanwork::EMITTERTYPE_Circle, false)
{
	mVelocity = _velocity;
	mCounter = 0.0f;
	mEmitter.parseParticleFile("resources/test.particle");
}

Bullet::~Bullet(void)
{
}

bool Bullet::update(Feanwork::Game* _game)
{
	mCounter += _game->getDelta();
	if(mCounter >= 6000.0f)
		destroy();

	addPosition(mVelocity.x * _game->getDelta(), mVelocity.y * _game->getDelta());
	return true;
}

bool Bullet::render(Feanwork::Game* _game)
{
	Object::render(_game);
	return true;
}

void Bullet::collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Feanwork::Game* _game)
{
	Feanwork::Emitter* emitter = &mEmitter;
	emitter->setPosition(mX, mY);
	emitter->setDirection(mVelocity.x, mVelocity.y);
	_game->addEmitter(emitter);
	destroy();
}

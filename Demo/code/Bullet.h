#ifndef BULLET_H_
#define BULLET_H_
#include "Object.h"
#include "Game.h"

class Emitter;
class Bullet : public Feanwork::Object
{
public:
	Bullet(int _resourceID, float _xPos, float _yPos, sf::Vector2f _velocity);
	~Bullet(void);

	virtual bool update(Feanwork::Game* _game);
	virtual bool render(Feanwork::Game* _game);
	virtual void collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Object* _collision, Feanwork::Game* _game);

protected:
	sf::Vector2f mVelocity;
	float		 mCounter;

	Feanwork::Emitter mEmitter;
};

#endif

#ifndef PLAYER_H_
#define PLAYER_H_
#include "Object.h"
#include "Game.h"
#include "Animation.h"

using namespace Feanwork;

class Player : public Feanwork::Object, public Feanwork::Animation
{
public: 
	Player(int _resourceID, float _xPos, float _yPos);
	~Player();

	bool update(Feanwork::Game* _game);
	bool render(Feanwork::Game* _game);
	void collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal, Feanwork::Game* _game);

protected:
	int mDelay;
};

#endif

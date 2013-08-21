#ifndef PLAYER_H_
#define PLAYER_H_
#include "Feanwork/Object.h"
#include "Feanwork/Game.h"

using namespace Feanwork;

class Player : public Feanwork::Object
{
public: 
	Player(int _resourceID, float _xPos, float _yPos);
	~Player();

	bool update(Feanwork::Game* _game);
	bool render(Feanwork::Game* _game);
	void collisionCallback(sf::Vector2f _depth, sf::Vector2f _normal);

protected:
	int mDelay;
};

#endif

#include <cmath>
#include "Collision.h"
#include "Object.h"
#include "Game.h"

namespace Feanwork
{
	bool Collision::checkCollides(Object* _first, Object* _second, Game* _game)
	{
		sf::Vector2f depth(0, 0);
		sf::Vector2f normal(0, 0);

		sf::IntRect first  = _first->getAABB();
		sf::IntRect second = _second->getAABB();
		sf::Vector2i firstPos((int)_first->getX() + first.width, (int)_first->getY() + first.height);
		sf::Vector2i secondPos((int)_second->getX() + second.width, (int)_second->getY() + second.height);

		int dx  =  firstPos.x - secondPos.x;
		int dy  =  firstPos.y - secondPos.y;
		int adx =  abs(firstPos.x - secondPos.x);
		int ady =  abs(firstPos.y - secondPos.y);
		int sw  = (first.width  + second.width);
		int sh  = (first.height + second.height);

		if((adx < sw) && (ady < sh))
		{
			float invDist = 1.0f / (float)sqrt(dx * dx + dy * dy);
			normal = getNormal(sf::Vector2f(dx * invDist, dy * invDist));
			depth  = sf::Vector2f((float)abs(adx - sw), (float)abs(ady - sh));

			_first->collisionCallback(depth, normal, _game);
			_second->collisionCallback(depth, normal, _game);

			return true;
		}
		return false;
	}

	sf::Vector2f Collision::getNormal(sf::Vector2f _vector)
	{
		sf::Vector2f normal;

		normal.x =  1.0f;
		normal.y =  0.0f;

		if((normal.x * _vector.x + normal.y * _vector.y) > .7f)
			return normal;

		normal.x = -1.0f;
		normal.y =  0.0f;

		if((normal.x * _vector.x + normal.y * _vector.y) > .7f)
			return normal;

		normal.x =  0.0f;
		normal.y =  1.0f;

		if((normal.x * _vector.x + normal.y * _vector.y) > .7f)
			return normal;

		normal.x =  0.0f;
		normal.y = -1.0f;

		if((normal.x * _vector.x + normal.y * _vector.y) > .7f)
			return normal;
	}
}

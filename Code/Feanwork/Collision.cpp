#include <cmath>
#include "Collision.h"
#include "Object.h"

namespace Feanwork
{
	bool Collision::checkCollides(Object* _first, Object* _second)
	{
		sf::Vector2f depth(0, 0);
		sf::Vector2f normal(0, 0);

		sf::IntRect first  = _first->getAABB();
		sf::IntRect second = _second->getAABB();

		int dx  =  first.left - second.left;
		int dy  =  first.top  - second.top;
		int adx =  abs(first.left - second.left);
		int ady =  abs(first.top - second.top);
		int sw  = (first.width + second.width);
		int sh  = (first.height + second.height);

		if((adx < sw) && (ady < sh))
		{
			float invDist = 1.0f / (float)sqrt(dx * dx + dy * dy);
			normal = getNormal(sf::Vector2f(dx * invDist, dy * invDist));
			depth  = sf::Vector2f((float)abs(adx - sw), (float)abs(ady - sh));

			_first->collisionCallback(depth, normal);
			_second->collisionCallback(depth, normal);

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

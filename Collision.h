#ifndef COLLISION_H_
#define COLLISION_H_
#include <SFML/Graphics.hpp>

namespace Feanwork
{
	class Object;
	class Collision
	{
	public:
		Collision() {}
		~Collision() {}

		bool		 checkCollides(Object* _first, Object* _second);
		sf::Vector2f getNormal(sf::Vector2f _vector);
	};
}

#endif

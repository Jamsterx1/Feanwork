#ifndef COLLISION_H_
#define COLLISION_H_
#include <SFML/Graphics.hpp>

namespace Feanwork
{
	class Game;
	class Object;
	class Collision
	{
	public:
		Collision() {}
		~Collision() {}

		bool		 checkCollides(Object* _first, Object* _second, Game* _game);
		sf::Vector2f getNormal(sf::Vector2f _vector);
	};
}

#endif

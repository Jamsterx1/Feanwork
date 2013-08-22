#ifndef _RANDOM_H
#define _RANDOM_H

#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

class Random
{
public:
	inline float random()
	{
		return rand() / (float)RAND_MAX;
	}

	inline float rand_range(float min, float max)
	{
		if(min > max)
			std::swap(min, max);

		return ((random() * (max - min)) + min);
	}

	inline sf::Vector2f rand_unit_vec()
	{
		float x = (random() * 2.0f) - 1.0f;
		float y = (random() * 2.0f) - 1.0f;

		float length = sqrt(x*x + y*y);

		if(length != 0)
		{
			x = x / length;
			y = y / length;
		}

		return sf::Vector2f(x, y);
	}

	static Random* get_singleton()
	{
		static Random random;
		return &random;
	}

private:
	Random(){};
	~Random(){};
};

#endif
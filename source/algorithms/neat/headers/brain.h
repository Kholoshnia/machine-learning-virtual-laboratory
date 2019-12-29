#pragma once

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <vector>

namespace neat
{
	extern float mutation_rate;
	extern int directions_array_size;

	struct Brain
	{
		int step;
		std::vector<sf::Vector2f>directions;

		Brain();
		Brain clone();
		void mutate();
		void randomize();
		void movement(sf::Vector2f& obj);
	};
}
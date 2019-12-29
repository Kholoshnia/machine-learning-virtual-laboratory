#pragma once

#include <SFML/Graphics.hpp>

extern int width, height;

namespace ql
{
	extern float thickness;
	extern sf::Vector2i map_size;

	struct Agent
	{
		sf::Vector2f pos;
		sf::RectangleShape rect;

		Agent();
		void update(const int n);
		void show(sf::RenderWindow& window);
	};
}
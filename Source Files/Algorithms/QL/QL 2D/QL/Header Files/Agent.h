#pragma once

#include <SFML/Graphics.hpp>

extern sf::Vector2i map_size;

extern float thickness;
extern unsigned int width, height;

class Agent
{
public:
	sf::Vector2f pos;
	sf::RectangleShape rect;

	Agent();
	void update(const int n);
	void show(sf::RenderWindow& window);
};
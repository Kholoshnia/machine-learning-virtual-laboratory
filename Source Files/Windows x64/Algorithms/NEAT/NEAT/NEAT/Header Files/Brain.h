#pragma once

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <vector>

extern float mutation_rate;
extern unsigned int directions_array_size;

class Brain
{
public:
	unsigned int step;
	float mutation_rate;
	std::vector<sf::Vector2f> directions;

	Brain();
	Brain clone();
	void mutate();
	void randomize();
	void movement(sf::Vector2f& obj);
};
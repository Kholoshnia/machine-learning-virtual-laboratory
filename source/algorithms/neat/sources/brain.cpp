#include "NEAT/Header Files/NEAT_Brain.h"

neat::Brain::Brain()
{
	step = 0;
	directions.resize(directions_array_size);
	randomize();
}

void neat::Brain::randomize()
{
	for (auto& el : directions)
		movement(el);
}

neat::Brain neat::Brain::clone()
{
	Brain clone;
	clone.directions = directions;
	return clone;
}

void neat::Brain::mutate()
{
	for (auto& el : directions)
	{
		float randn = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
		if (randn < mutation_rate)
			movement(el);
	}
}

void neat::Brain::movement(sf::Vector2f& obj)
{
	obj.x = cos(-180.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 360.0f) * static_cast<float>(M_PI) / 180.0f);
	obj.y = sin(-180.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / 360.0f) * static_cast<float>(M_PI) / 180.0f);
}
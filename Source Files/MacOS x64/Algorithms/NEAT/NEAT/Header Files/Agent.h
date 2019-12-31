#pragma once

#include <SFML/Graphics.hpp>

#include "Map.h"
#include "Brain.h"

extern float max_speed;
extern std::shared_ptr<Map> map;

class Agent
{
public:
	Brain brain;

	sf::Color color;
	sf::CircleShape circle;
	sf::Vector2f pos, vel, acc;

	float fitness;
	bool dead, reached_goal, is_best;

	Agent();
	Agent get_copy();
	void move();
	void update();
	void calculate_fitness();
	void show(sf::RenderWindow& window);
};
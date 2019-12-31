#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>

#include "Agent.h"

extern unsigned int directions_array_size, population_quantity, layers_quantity;

struct Population
{
	sf::Color color;
	float fitness_sum;
	bool reached_the_goal;
	std::vector<Agent> agents;
	unsigned int gen, min_step, best_agent, after_reach;

	Population();
	Agent select_parent();
	bool all_agents_dead();
	void mutate();
	void update();
	void set_best_agent();
	void calculate_fitness();
	void natural_selection();
	void calculate_fitness_sum();
	void show(sf::RenderWindow& window);
};
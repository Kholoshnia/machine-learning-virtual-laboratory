#include "Population.h"

Population::Population() : reached_the_goal(false), gen(1), best_agent(0),
	fitness_sum(0), after_reach(0), min_step(directions_array_size)
{
	srand(clock());
	agents.resize(population_quantity);
	color = sf::Color(rand() & 255, rand() & 255, rand() & 255);
	if (layers_quantity > 1)
		for (auto& el : agents)
			el.circle.setFillColor(color);
}

Agent Population::select_parent()
{
	float random = float(rand()) / (float(RAND_MAX / fitness_sum));
	float running_sum = 0;
	for (auto& agent : agents)
	{
		running_sum += agent.fitness;
		if (running_sum > random)
			return agent;
	}
	Agent null;
	return null;
}

bool Population::all_agents_dead()
{
	for (auto& agent : agents)
		if (!agent.dead && !agent.reached_goal)
			return false;
	return true;
}

void Population::mutate()
{
	for (unsigned int i = 1; i < agents.size(); ++i)
		agents[i].brain.mutate();
}

void Population::update()
{
	for (auto& el : agents)
	{
		if (el.brain.step > min_step) el.dead = true;
		else el.update();
		if (el.reached_goal)
		{
			min_step = el.brain.step;
			reached_the_goal = true;
		}
	}
}

void Population::set_best_agent()
{
	best_agent = static_cast<int>(std::max_element(agents.begin(), agents.end(), [&](Agent& agent_1, Agent& agent_2)
	{ return agent_1.fitness < agent_2.fitness;}) - agents.begin());
}

void Population::calculate_fitness()
{
	for (auto& agent : agents)
		agent.calculate_fitness();
}

void Population::natural_selection()
{
	std::vector<Agent>new_agents;
	new_agents.resize(agents.size());
	new_agents[0] = agents[best_agent].get_copy();
	new_agents[0].is_best = true;
	for (unsigned int i = 1; i < agents.size(); ++i)
	{
		Agent parent = select_parent();
		new_agents[i] = parent.get_copy();
	}
	agents = new_agents;
	if (layers_quantity > 1)
		for (auto& el : agents)
			el.circle.setFillColor(color);
	agents[0].circle.setFillColor(sf::Color::Green);
	gen++;
}

void Population::calculate_fitness_sum()
{
	fitness_sum = 0;
	for (auto& agent : agents)
		fitness_sum += agent.fitness;
}

void Population::show(sf::RenderWindow& window)
{
	for (unsigned int i = 1; i < agents.size(); ++i)
		agents[i].show(window);
	agents[0].show(window);
}

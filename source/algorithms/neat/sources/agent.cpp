#include <NEAT/Header Files/NEAT_Agent.h>

neat::Agent::Agent()
{
	fitness = 0.0f;

	dead = false;
	is_best = false;
	reached_goal = false;

	pos = sf::Vector2f(static_cast<float>(map->pos_agent.x), static_cast<float>(map->pos_agent.y));
	vel = sf::Vector2f(0.0f, 0.0f);
	acc = sf::Vector2f(0.0f, 0.0f);

	circle.setFillColor(sf::Color::Black);
	circle.setRadius(agent_radius);
	circle.setOrigin(agent_radius, agent_radius);
}

neat::Agent neat::Agent::get_copy()
{
	Agent copy;
	copy.brain = brain.clone();
	return copy;
}

void neat::Agent::move()
{
	if (brain.directions.size() > brain.step)
	{
		acc = brain.directions[brain.step];
		brain.step++;
	}
	else dead = true;
	if (abs(vel.x + acc.x) < max_speed) vel.x += acc.x;
	if (abs(vel.y + acc.y) < max_speed) vel.y += acc.y;
	pos += vel;
}

void neat::Agent::update()
{
	if (!dead && !reached_goal)
	{
		move();
		if (map->touched_additional_reward(pos)) fitness++;
		else if (map->touched_wall(pos)) dead = true;
		else if (map->touched_goal(pos)) reached_goal = true;
	}
}

void neat::Agent::calculate_fitness()
{
	if (reached_goal) fitness = brain.directions.size() + 1.0f / static_cast<float>(brain.step * brain.step);
	else fitness = 1.0f / (map->dist(pos) * map->dist(pos));
}

void neat::Agent::show(sf::RenderWindow& window)
{
	circle.setPosition(pos);
	window.draw(circle);
}
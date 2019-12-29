#include <NEAT/Header Files/NEAT_Map.h>

neat::Map::Map()
{
	map_markup.resize(map_size.y);

	pos_goal = sf::Vector2f(400, 400);
	pos_agent = sf::Vector2f(400, 300);

	circle_goal.setRadius(goal_radius);
	circle_goal.setFillColor(sf::Color::Red);
	circle_goal.setOrigin(goal_radius, goal_radius);
	circle_goal.setPosition(static_cast<float>(pos_goal.x), static_cast<float>(pos_goal.y));

	circle_agent.setRadius(agent_radius);
	circle_agent.setFillColor(sf::Color::Black);
	circle_agent.setOrigin(agent_radius, agent_radius);
	circle_agent.setPosition(static_cast<float>(pos_agent.x), static_cast<float>(pos_agent.y));

	rect.setSize(sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y)));
}

void neat::Map::from_file()
{
	for (int i = 0; i < map_size.y; i++)
		for (int j = 0; j < map_size.x; j++)
			if (map_markup[i][j] == 'W')
				pos_rects.emplace_back(static_cast<float>(j * (width / map_size.x)), static_cast<float>(i * (height / map_size.y)));
			else if (map_markup[i][j] == 'G')
				pos_goal = sf::Vector2f(static_cast<float>(j * (width / map_size.x)), static_cast<float>(i * (height / map_size.y)));
			else if (map_markup[i][j] == 'A')
				pos_agent = sf::Vector2f(static_cast<float>(j * (width / map_size.x)), static_cast<float>(i * (height / map_size.y)));
			else if (map_markup[i][j] == 'B')
				pos_additional_rewards.emplace_back(static_cast<float>(j * (width / map_size.x)), static_cast<float>(i * (height / map_size.y)));
}

float neat::Map::dist(sf::Vector2f& obj) { return sqrt((pos_goal.x - obj.x) * (pos_goal.x - obj.x) + (pos_goal.y - obj.y) * (pos_goal.y - obj.y)); }

bool neat::Map::touched_wall(sf::Vector2f& obj)
{
	for (auto& el : pos_rects)
		if (sf::FloatRect(el, sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y))).intersects(sf::FloatRect(obj, sf::Vector2f(agent_radius, agent_radius))))
			return true;
	return false;
}

bool neat::Map::touched_goal(sf::Vector2f& obj) { return dist(obj) < goal_radius + agent_radius; }

bool neat::Map::touched_additional_reward(sf::Vector2f& obj)
{
	for (auto& el : pos_additional_rewards)
		if (sf::FloatRect(el, sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y))).intersects(sf::FloatRect(obj, sf::Vector2f(agent_radius, agent_radius))))
			return true;
	return false;
}

void neat::Map::show(sf::RenderWindow& window)
{
	rect.setFillColor(sf::Color::Blue);
	for (auto& el : pos_rects)
	{
		rect.setPosition(el);
		window.draw(rect);
	}

	rect.setFillColor(sf::Color::Yellow);
	for (auto& el : pos_additional_rewards)
	{
		rect.setPosition(el);
		window.draw(rect);
	}

	circle_goal.setPosition(static_cast<float>(pos_goal.x), static_cast<float>(pos_goal.y));
	circle_goal.setRadius(goal_radius);
	circle_goal.setOrigin(goal_radius, goal_radius);
	window.draw(circle_goal);

	circle_agent.setPosition(static_cast<float>(pos_agent.x), static_cast<float>(pos_agent.y));
	circle_agent.setRadius(agent_radius);
	circle_agent.setOrigin(agent_radius, agent_radius);
	window.draw(circle_agent);
}
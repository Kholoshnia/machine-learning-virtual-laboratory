#include "Map.h"

Map::Map()
{
	pos_goal = sf::Vector2f(static_cast<float>(width / 2), static_cast<float>(height / 2));
	pos_agent = sf::Vector2f(static_cast<float>(width / 2), static_cast<float>(height / 4));

	circle_goal.setRadius(radius_goal);
	circle_goal.setFillColor(sf::Color::Red);
	circle_goal.setOrigin(radius_goal, radius_goal);
	circle_goal.setPosition(static_cast<float>(pos_goal.x), static_cast<float>(pos_goal.y));

	circle_agent.setRadius(radius_agent);
	circle_agent.setFillColor(sf::Color::Black);
	circle_agent.setOrigin(radius_agent, radius_agent);
	circle_agent.setPosition(static_cast<float>(pos_agent.x), static_cast<float>(pos_agent.y));

	rect.setSize(sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y)));
}

void Map::from_file()
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

float Map::dist(sf::Vector2f& obj) { return sqrt((pos_goal.x - obj.x) * (pos_goal.x - obj.x) + (pos_goal.y - obj.y) * (pos_goal.y - obj.y)); }

bool Map::touched_wall(sf::Vector2f& obj)
{
	for (auto& el : pos_rects)
		if (sf::FloatRect(el, sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y))).intersects(sf::FloatRect(obj, sf::Vector2f(radius_agent, radius_agent))))
			return true;
	return false;
}

bool Map::touched_goal(sf::Vector2f& obj) { return dist(obj) < radius_goal + radius_agent; }

bool Map::touched_additional_reward(sf::Vector2f& obj)
{
	for (auto& el : pos_additional_rewards)
		if (sf::FloatRect(el, sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y))).intersects(sf::FloatRect(obj, sf::Vector2f(radius_agent, radius_agent))))
			return true;
	return false;
}

void Map::show(sf::RenderWindow& window)
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
	circle_goal.setRadius(radius_goal);
	circle_goal.setOrigin(radius_goal, radius_goal);
	window.draw(circle_goal);

	circle_agent.setPosition(static_cast<float>(pos_agent.x), static_cast<float>(pos_agent.y));
	circle_agent.setRadius(radius_agent);
	circle_agent.setOrigin(radius_agent, radius_agent);
	window.draw(circle_agent);
}
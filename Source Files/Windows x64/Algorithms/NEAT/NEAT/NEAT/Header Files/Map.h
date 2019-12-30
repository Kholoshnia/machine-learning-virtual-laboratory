#pragma once

#include <SFML/Graphics.hpp>

extern sf::Vector2i map_size;
extern unsigned int width, height;
extern float radius_agent, radius_goal;

class Map
{
public:
	sf::RectangleShape rect;
	sf::CircleShape circle_goal, circle_agent;
	sf::Vector2f map_size, pos_goal, pos_agent;

	std::vector<std::string> map_markup;
	std::vector<sf::Vector2f> pos_rects, pos_additional_rewards;

	Map();
	void from_file();
	float dist(sf::Vector2f& obj);
	bool touched_wall(sf::Vector2f& obj);
	bool touched_goal(sf::Vector2f& obj);
	bool touched_additional_reward(sf::Vector2f& obj);
	void show(sf::RenderWindow& window);
};
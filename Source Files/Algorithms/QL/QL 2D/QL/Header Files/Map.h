#pragma once

#include <SFML/Graphics.hpp>

#include <fstream>

extern std::string path;
extern unsigned int width, height;

extern float thickness;
extern sf::Vector2i map_size;
extern std::vector<unsigned int> initials;

class Map
{
public:
	sf::RectangleShape space, wall, goal;
	std::vector<std::vector<char>> map_markup;
	std::vector<sf::Vector2f> pos_spaces, pos_walls;

	Map();
	void save();
	void update();
	void show(sf::RenderWindow& window);
};
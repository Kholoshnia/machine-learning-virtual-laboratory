#include "Map.h"

Map::Map()
{
	space.setOutlineThickness(thickness);
	space.setFillColor(sf::Color::White);
	space.setOutlineColor(sf::Color::Black);
	space.setSize(sf::Vector2f(static_cast<float>(width / map_size.x) - thickness, static_cast<float>(height / map_size.y) - thickness));

	wall.setOutlineThickness(thickness);
	wall.setFillColor(sf::Color::Blue);
	wall.setOutlineColor(sf::Color::Black);
	wall.setSize(sf::Vector2f(static_cast<float>(width / map_size.x) - thickness, static_cast<float>(height / map_size.y) - thickness));

	goal.setOutlineThickness(thickness);
	goal.setFillColor(sf::Color::Red);
	goal.setOutlineColor(sf::Color::Black);
	goal.setSize(sf::Vector2f(static_cast<float>(width / map_size.x) - thickness, static_cast<float>(height / map_size.y) - thickness));

	map_markup.resize(map_size.y);
	for (auto& el : map_markup)
		for (unsigned int i = 0; i < map_size.x; i++)
			el.emplace_back('S');

	update();
}

void Map::save()
{
	map_markup[static_cast<int>(goal.getPosition().y) / (width / map_size.y)][static_cast<int>(goal.getPosition().x) / (height / map_size.x)] = 'G';
	update();
}

void Map::update()
{
	initials.clear();
	pos_walls.clear();
	pos_spaces.clear();

	int k = 0;
	for (int y = 0; y < map_size.y; y++)
		for (int x = 0; x < map_size.x; x++)
		{
			if (map_markup[y][x] == 'S')
			{
				pos_spaces.emplace_back(static_cast<float>(x * (height / map_size.x)), static_cast<float>(y * (width / map_size.y)));
				initials.emplace_back(k);
			}
			else if (map_markup[y][x] == 'W')
				pos_walls.emplace_back(static_cast<float>(x * (height / map_size.x)), static_cast<float>(y * (width / map_size.y)));
			else if (map_markup[y][x] == 'G')
				goal.setPosition(static_cast<float>(x * (height / map_size.x)), static_cast<float>(y * (width / map_size.y)));

			k++;
		}

	wall.setSize(sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y)));
	goal.setSize(sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y)));
	space.setSize(sf::Vector2f(static_cast<float>(width / map_size.x), static_cast<float>(height / map_size.y)));
}

void Map::show(sf::RenderWindow& window)
{
	for (auto& el : pos_spaces)
	{
		space.setPosition(el);
		window.draw(space);
	}

	for (auto& el : pos_walls)
	{
		wall.setPosition(el);
		window.draw(wall);
	}

	window.draw(goal);
}
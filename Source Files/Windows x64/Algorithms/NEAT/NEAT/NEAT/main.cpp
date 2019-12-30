#include <SFML/Graphics.hpp>
#include <fstream>
#include <memory>
#include <vector>

#include <Enums.h>

#include "Map.h"
#include "Layers.h"
#include "Population.h"

Modes mode;

std::string path;

sf::Font font;
sf::Image icon;
sf::Sprite loading;
sf::Texture loading_texture;
std::string path_input, path_output;

bool from_image, check_from_file, map_loaded, result_loaded, output_path_set, pause, show_controls;

VisualizationTypes visualization_type;

std::shared_ptr<Map> map;
std::shared_ptr<Layers> layers;
std::shared_ptr<Population> population;

std::fstream fout, fin;
std::vector<sf::Vector2f> pos;
std::vector<std::string> map_markup;

sf::RectangleShape rect;
sf::Text text[4], controls[3];
sf::Vector2i map_size, wall_size, pos_agent, pos_goal;

bool around, map_changed;
float radius_agent, radius_goal, max_speed, mutation_rate;
unsigned int width, height, directions_array_size, population_quantity, layers_quantity, auto_exit;

int main()
{
	
}
#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <Windows.h>

#include <Enums.h>

#include <NEAT/Header Files/NEAT_Map.h>
#include <NEAT/Header Files/NEAT_Brain.h>
#include <NEAT/Header Files/NEAT_Layers.h>
#include <NEAT/Header Files/NEAT_Population.h>

extern Modes mode;
extern Dimensions dimension;

extern int width, height;
extern sf::Sprite loading;
extern std::string path, path_input, path_output;
extern bool from_image, map_loaded, result_loaded, pause, output_path_set, show_controls;

namespace neat
{
	extern VisualizationTypes visualization_type;

	extern std::shared_ptr<Map> map;
	extern std::shared_ptr<Layers> layers;
	extern std::shared_ptr<Population> population;

	extern std::fstream fout, fin;
	extern std::vector<std::string> map_markup;

	extern sf::RectangleShape rect;
	extern sf::Text text[4], controls[3];
	extern sf::Vector2i map_size, wall_size;

	extern bool around, map_changed;
	extern int population_quantity, layers_quantity, directions_array_size, auto_exit;

	void check_2d();
	void check_3d();
	void create_new_map_2d();
	void create_new_map_3d();
	void set_result_file_path();
	void load_map_from_file_2d();
	void load_map_from_file_3d();
	void load_result_from_file_2d();
	void load_result_from_file_3d();
	void with_visualization_2d();
	void with_visualization_3d();
	void without_visualization_2d();
	void without_visualization_3d();
	void draw(sf::Event& event);
}
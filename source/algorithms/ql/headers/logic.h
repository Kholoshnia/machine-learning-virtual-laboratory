#pragma once

#include <SFML/Graphics.hpp>

#include <sstream>
#include <fstream>
#include <Windows.h>

#include <Enums.h>

#include <QL/Header Files/QL_Map.h>
#include <QL/Header Files/QL_Table.h>
#include <QL/Header Files/QL_Agent.h>

extern sf::Image icon;
extern int width, height;
extern sf::Sprite loading;
extern std::string path_input, path_output;
extern bool from_image, map_loaded, result_loaded, output_path_set, show_controls;

namespace ql
{
	extern std::shared_ptr<Map> map;
	extern std::shared_ptr<Table> table;
	extern std::shared_ptr<Agent> agent;

	extern sf::Vector2i map_size;
	extern std::fstream fout, fin;
	extern bool goal_loaded, around;
	extern std::vector<int> initials;
	extern long long int goal_reward;
	extern int goal_state, repetitions;
	extern sf::Text text[4], controls[3];
	extern std::vector<std::vector<long long int>> temp_Q;

	void check_2d();
	void check_3d();
	void create_new_map_2d();
	void create_new_map_3d();
	void set_result_file_path();
	void draw(sf::Event& event);
	void load_map_from_file_2d();
	void load_map_from_file_3d();
	void load_result_from_file_2d();
	void load_result_from_file_3d();
	void with_visualization_2d();
	void with_visualization_3d();
	void without_visualization_2d();
	void without_visualization_3d();
}
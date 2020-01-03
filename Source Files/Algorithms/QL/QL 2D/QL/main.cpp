#ifdef _WIN64
   //define something for Windows (32-bit)
#elif _WIN32
   //define something for Windows (32-bit)
#elif __APPLE__
    // define something for OSX
#elif __linux
    // linux
#elif __unix
    // Unix
#elif __posix
    // POSIX
#endif

#include <SFML/Graphics.hpp>

#include <cmath>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>

#include "Enums.h"

#include "Map.h"
#include "Table.h"
#include "Agent.h"

#pragma region Variables
Modes mode;
VisualizationTypes visualization_type;

std::shared_ptr<Map> map;
std::shared_ptr<Table> table;
std::shared_ptr<Agent> agent;

sf::Font font;
sf::Image icon;
sf::Sprite loading;
sf::Texture loading_texture;

std::fstream f_out, f_in;
std::string path_info, path_input, path_output, str_input;

sf::Vector2i map_size;
std::vector<std::vector<char>> map_markup;
std::vector<std::vector<long long int>> temp_Q;

std::vector<unsigned int> initials;
sf::Text text[4], controls[3];

float gamma_parameter, thickness;
long long int goal_reward;
unsigned int goal_state, iterations, repetitions, width, height;
bool from_image, check_from_file, map_loaded, result_loaded, output_path_set, pause, show_controls, goal_loaded, around;
#pragma endregion

#pragma region Functions
void initialize_variables(void)
{

#ifdef _WIN64
    font.loadFromFile("Resource Files/Fonts/sans-serif.ttf");
    loading_texture.loadFromFile("Resource Files/Textures/loading.png");
    icon.loadFromFile("icon.png");
#elif _WIN32
    font.loadFromFile("Resource Files/Fonts/sans-serif.ttf");
    loading_texture.loadFromFile("Resource Files/Textures/loading.png");
    icon.loadFromFile("icon.png");
#elif __APPLE__
    font.loadFromFile("/Users/vadim/Documents/GitHub/mlvl/Source Files/Algorithms/QL/QL 2D/QL/Resource Files/Fonts/sans-serif.ttf");
    loading_texture.loadFromFile("/Users/vadim/Documents/GitHub/mlvl/Source Files/Algorithms/QL/QL 2D/QL/Resource Files/Textures/loading.png");
    icon.loadFromFile("/Users/vadim/Documents/GitHub/mlvl/Source Files/Algorithms/QL/QL 2D/QL/icon.png");
#endif

	loading.setOrigin(100, 25);
	loading.setPosition(400, 400);
	loading.setTexture(loading_texture);

	mode = Modes::LEARN;

	width = 800;
	height = 800;
	pause = false;
	from_image = false;
	map_loaded = false;
	show_controls = false;
	result_loaded = false;
	check_from_file = false;
	output_path_set = false;

	visualization_type = VisualizationTypes::ITERATIONS;

	gamma_parameter = 0.8f;
	repetitions = 5;
	map_size.x = 10;
	map_size.y = 10;
	thickness = 2.5f;
	goal_reward = 100;

	around = false;
	goal_loaded = false;

	text[0].setPosition(15, 10);
	text[0].setString(L"Position: ");
	text[1].setPosition(100, 10);
	text[1].setString(L"0");
	text[2].setPosition(15, 30);
	text[2].setString(L"Repetition: ");
	text[3].setPosition(117, 30);
	text[3].setString(L"0");

	for (unsigned int i = 0; i < 4; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(20);
		text[i].setOutlineThickness(3);
		text[i].setOutlineColor(sf::Color::White);
		text[i].setFillColor(sf::Color::Black);
	}

	controls[0].setPosition(685, 10);
	controls[0].setString(L"[Esc] - Exit");
	controls[1].setPosition(612, 10);
	controls[1].setString(L"[H] - Show controls");
	controls[2].setPosition(612, 10);
	controls[2].setString(L"[H] - Show controls");

	for (unsigned int i = 0; i < 3; i++)
	{
		controls[i].setFont(font);
		controls[i].setCharacterSize(20);
		controls[i].setOutlineThickness(3);
		controls[i].setOutlineColor(sf::Color::White);
		controls[i].setFillColor(sf::Color::Black);
	}
}

void draw(sf::Event& event)
{
	if (event.type == event.KeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && map->pos_walls.size() > 0)
		{
			map->map_markup[static_cast<int>(map->pos_walls[map->pos_walls.size() - 1].y) / (height / map_size.y)][static_cast<int>(map->pos_walls[map->pos_walls.size() - 1].x) / (width / map_size.x)] = 'S';
			map->update();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !around)
		{
			around = true;

			for (int y = 0; y < map_size.y; y++)
				for (int x = 0; x < map_size.x; x++)
					if (x == 0 || x == map_size.x - 1 || y == 0 || y == map_size.y - 1)
						map->map_markup[y][x] = 'W';

			map->update();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			around = false;

			map->map_markup[static_cast<int>(map->goal.getPosition().y) / (height / map_size.y)][static_cast<int>(map->goal.getPosition().x) / (width / map_size.x)] = 'G';

			for (int y = 0; y < map_size.y; y++)
				map->map_markup[y].emplace_back('S');

			std::vector<char> line;
			for (int x = 0; x < map_size.x + 1; x++)
				line.emplace_back('S');
			map->map_markup.emplace_back(line);

			map_size.x += 1;
			map_size.y += 1;
			map->update();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && map_size.x > 2 && map_size.y > 2)
		{
			if (map->goal.getPosition().x + map->goal.getSize().x > static_cast<float>((height / map_size.y) * map_size.y) || map->goal.getPosition().y + map->goal.getSize().y > static_cast<float>((width / map_size.x) * map_size.x))
			{
				map->map_markup[0][0] = 'S';
				map->goal.setPosition(0.0f, 0.0f);
			}
			else map->map_markup[static_cast<int>(map->goal.getPosition().y) / (height / map_size.y)][static_cast<int>(map->goal.getPosition().x) / (width / map_size.x)] = 'G';

			around = false;

			for (int y = 0; y < map_size.y; y++)
				map->map_markup[y].pop_back();
			map->map_markup.pop_back();
			map_size.x -= 1;
			map_size.y -= 1;
			map->update();
		}
	}

	if (event.type == event.MouseMoved)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.mouseMove.x < (static_cast<int>(height) / map_size.x) * map_size.x && event.mouseMove.y < (static_cast<int>(width) / map_size.y) * map_size.y)
		{
			map->map_markup[static_cast<int>(map->goal.getPosition().y) / (height / map_size.y)][static_cast<int>(map->goal.getPosition().x) / (width / map_size.x)] = 'G';

			if ([&]()
				{
					for (auto& el : map->pos_walls)
						if (el == sf::Vector2f(static_cast<float>(event.mouseMove.x / (height / map_size.x)), static_cast<float>(event.mouseMove.y / (width / map_size.y))))
							return false;
						return true;
				}())
				map->map_markup[event.mouseMove.y / (height / map_size.y)][event.mouseMove.x / (width / map_size.x)] = 'W';
					map->update();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && event.mouseMove.x < (static_cast<int>(height) / map_size.x) * map_size.x && event.mouseMove.y < (static_cast<int>(width) / map_size.y) * map_size.y)
		{
			map->map_markup[static_cast<int>(map->goal.getPosition().y) / (height / map_size.y)][static_cast<int>(map->goal.getPosition().x) / (width / map_size.x)] = 'S';
			map->goal.setPosition(static_cast<float>(event.mouseMove.x / (width / map_size.x) * (width / map_size.x)), static_cast<float>(event.mouseMove.y / (height / map_size.y) * (height / map_size.y)));
			map->update();
		}
	}
}

void check()
{
	agent.reset(new Agent());

	sf::RenderWindow window(sf::VideoMode(width, height), "Check");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	std::vector<int> moves;

	int position, k = 0;
	bool done = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == event.MouseMoved)
			{
				moves.clear();

				position = map_size.y * (event.mouseMove.y / (width / map_size.y)) + event.mouseMove.x / (width / map_size.x);
				moves.emplace_back(position);
				if (std::find(initials.begin(), initials.end(), position) != initials.end())
				{
					moves.emplace_back(position);
					while (true)
					{
						int best_action = static_cast<int>(table->maximum(position, true));
						moves.emplace_back(best_action);
						if (best_action == goal_state) break;
						else position = best_action;
						if (moves.size() > static_cast<unsigned int>(map_size.x * map_size.y))
							break;
					}
					done = true;
				}

				moves.pop_back();
			}

			if (event.type == event.KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					window.close();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
				{
					show_controls = !show_controls;
					if (show_controls)
					{
						controls[1].setPosition(612, 10);
						controls[1].setString(L"[H] - Show controls");
					}
					else
					{
						controls[1].setPosition(552, 10);
						controls[1].setString(L"[LMB] - Choose state\n[Esc] - Exit\n[H] - Hide controls");
					}
				}
			}
		}

		window.clear();
		map->show(window);

		if (done)
		{
			for (auto& el : moves)
			{
				agent->update(el);
				agent->show(window);
			}

			if (k > moves.size() - 1)
			{
				moves.clear();
				k = 0;
				done = false;
			}
		}

		window.draw(controls[1]);
		window.display();
	}
}

void create_new_map()
{
	map.reset(new Map);

	sf::RenderWindow window(sf::VideoMode(width, height), "Map creator");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			draw(event);

			if (event.type == event.KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					window.draw(loading);
					window.display();

					map->save();

					f_out.open(path_input);
					if (f_out.is_open())
					{
						f_out.clear();
						f_out << "map-size:;" << map_size.x << ';' << map_size.y << std::endl;

						for (int y = 0; y < map_size.y; y++)
						{
							for (int x = 0; x < map_size.x; x++)
								f_out << map->map_markup[y][x] << ';';
							f_out << std::endl;
						}

						f_out.close();
						window.close();
						map_loaded = true;
					}
					else
					{
						// if file is not opened
						exit(0);
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
				{
					show_controls = !show_controls;
					if (show_controls)
					{
						controls[2].setPosition(612, 10);
						controls[2].setString(L"[H] - Show controls");
					}
					else
					{
						controls[2].setPosition(552, 10);
						controls[2].setString(L"[LShift] - Move goal\n[Up][Down] - Resize map\n[Tab] - Fill around\n[LMB] - Draw map\n[RCtrl] - Erase map\n[Esc] - Exit without saving\n[Enter] - save&exit\n[H] - Hide controls");
					}
				}
			}
		}

		window.clear(sf::Color::White);

		map->show(window);
		window.draw(controls[2]);

		window.display();
	}

	goal_reward = static_cast<long long>(std::pow(map_size.x * map_size.y, 3));
	table.reset(new Table());
	map_loaded = true;
}

void load_map_from_file()
{
	std::string str;
	for (int i = static_cast<int>(path_input.length()) - 1; i >= 0; i--)
		if (path_input[i] != '.') str += path_input[i];
		else break;

	if (str == "pmb" || str == "gnp" || str == "agt" || str == "gpj" ||
		str == "fig" || str == "dsp" || str == "rdh" || str == "cip") from_image = true;
	else from_image = false;

	if (from_image)
	{
		sf::Image map_image;
		map_image.loadFromFile(path_input);
		map_size.x = static_cast<int>(map_image.getSize().x) / 10;
		map_size.y = static_cast<int>(map_image.getSize().y) / 10;
		map.reset(new Map());

		for (int y = 0; y < map_size.y; y++)
			for (int x = 0; x < map_size.x; x++)
				if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y* (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Red && !goal_loaded)
				{
					map->map_markup[y][x] = 'G';
					goal_loaded = true;
				}
				else if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y* (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Blue)
					map->map_markup[y][x] = 'W';
				else
					map->map_markup[y][x] = 'S';

		if (path_input != "")
			map_loaded = true;
	}
	else
	{
		map_size.x = 10;
		map_size.y = 10;
		map.reset(new Map);

		f_in.open(path_input);
		if (f_in.is_open())
		{
			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input, ';');
			map_size.x = std::stoi(str_input);

			std::getline(f_in, str_input);
			map_size.y = std::stoi(str_input);

			map->map_markup.resize(map_size.y);
			for (int y = 0; y < map_size.y; y++)
				map->map_markup[y].resize(map_size.x);

			for (int y = 0; y < map_size.y; y++)
			{
				std::getline(f_in, str_input);
				str_input.erase(std::remove(str_input.begin(), str_input.end(), ';'), str_input.end());
				for (int x = 0; x < map_size.x; x++)
					map->map_markup[y][x] = str_input[x];
			}

			f_in.close();

			map_loaded = true;
		}
		else
		{
			// if file is not opened
			exit(0);
		}
	}

	map->update();
	goal_reward = static_cast<long long>(std::pow(map_size.x * map_size.y, 3));
	table.reset(new Table);
}

void load_result_from_file()
{
	f_in.open(path_output);
	if (f_in.is_open())
	{
		std::string line;
		for (int y = 0; y < map_size.x * map_size.y; y++)
			for (int x = 0; x < map_size.x * map_size.y; x++)
			{
				std::getline(f_in, line, ';');
				line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
				table->Q[y][x] = std::stoll(line);
			}
		f_in.close();
	}
	else
	{
		// if file is not opened
		exit(0);
	}

	result_loaded = true;
}

void with_visualization()
{
	agent.reset(new Agent());

	sf::RenderWindow window(sf::VideoMode(width, height), "Learn");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	unsigned int repetitions_k = 0, initials_k = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == event.KeyPressed)
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					window.close();
		}

		table->episode(initials[initials_k]);

		std::ostringstream initials_str;
		initials_str << initials[initials_k];
		text[1].setString(initials_str.str());

		std::ostringstream repetitions_str;
		repetitions_str << repetitions_k;
		text[3].setString(repetitions_str.str());

		initials_k++;

		if (initials_k > initials.size() - 1)
		{
			initials_k = 0;
			repetitions_k++;
		}

		window.clear();

		map->show(window);
		agent->show(window);

		for (unsigned int i = 0; i < 4; i++)
			window.draw(text[i]);

		window.draw(controls[0]);

		window.display();

		if (repetitions_k > repetitions - 1)
		{
			f_out.open(path_output);
			if (f_out.is_open())
			{
				f_out.clear();
				window.draw(loading);
				window.display();

				for (int y = 0; y < map_size.x * map_size.y; y++)
				{
					for (int x = 0; x < map_size.x * map_size.y; x++)
						f_out << table->Q[y][x] << ';';
					f_out << std::endl;
				}
				f_out.close();

				window.close();
			}
			else
			{
				// if file is not opened
				exit(0);
			}
		}
	}
}

void without_visualization()
{
	table.reset(new Table());
	agent.reset(new Agent());

	for (unsigned int i = 0; i < repetitions; i++)
		for (auto& el : initials)
			table->episode(el);

	f_out.open(path_input);
	if (f_out.is_open())
	{
		f_out.clear();
		for (int y = 0; y < map_size.x * map_size.y; y++)
		{
			for (int x = 0; x < map_size.x * map_size.y; x++)
				f_out << table->Q[y][x] << ';';
			f_out << std::endl;
		}
		f_out.close();
	}
	else
	{
		// if file is not opened
		exit(0);
	}
}

void load_parameters_from_info_file()
{
#ifdef _WIN64
    path_info = "Resource Files/Data/info-learn.csv";
#elif _WIN32
    path_info = "Resource Files/Data/info-learn.csv";
#elif __APPLE__
    path_info = "/Users/vadim/Documents/GitHub/mlvl/Source Files/Algorithms/QL/QL 2D/QL/Resource Files/Data/info-create.csv";
#endif
    
	f_in.open(path_info);
	if (f_in.is_open())
	{
		std::getline(f_in, str_input, ';');
		std::getline(f_in, str_input);
		if (str_input == "CREATE") mode = Modes::CREATE;
		else if (str_input == "LEARN") mode = Modes::LEARN;
		else if (str_input == "CHECK") mode = Modes::CHECK;

		if (mode == Modes::CREATE)
		{
			std::getline(f_in, str_input, ';');
			std::getline(f_in, path_input);
		}
		else if (mode == Modes::LEARN)
		{
			std::getline(f_in, str_input, ';');
			std::getline(f_in, path_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, path_output);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			if (str_input == "ITERATIONS") visualization_type = VisualizationTypes::ITERATIONS;
			else if (str_input == "ITERATIONS") visualization_type = VisualizationTypes::STATES;
			else if (str_input == "WITHOUT") visualization_type = VisualizationTypes::WITHOUT;

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			gamma_parameter = std::stof(str_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			iterations = std::stoi(str_input);
			if (iterations == 0) iterations = map_size.x * map_size.y;

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			repetitions = std::stoi(str_input);
		}
		else if (mode == Modes::CHECK)
		{
			std::getline(f_in, str_input, ';');
			std::getline(f_in, path_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, path_output);
		}

		f_in.close();
	}
	else
	{
		// if file is not opened
		exit(0);
	}
}
#pragma endregion

int main()
{
	initialize_variables();
	load_parameters_from_info_file();
	
	if (mode == Modes::CREATE)
		create_new_map();
	else if (mode == Modes::LEARN)
	{
		load_map_from_file();
		if (map_loaded)
			if (visualization_type == VisualizationTypes::ITERATIONS || visualization_type == VisualizationTypes::STATES)
				with_visualization();
			else if (visualization_type == VisualizationTypes::WITHOUT)
				without_visualization();
	}
	else if (mode == Modes::CHECK)
	{
		load_map_from_file();
		load_result_from_file();
		if (map_loaded && result_loaded)
			check();
	}

	return 0;
}

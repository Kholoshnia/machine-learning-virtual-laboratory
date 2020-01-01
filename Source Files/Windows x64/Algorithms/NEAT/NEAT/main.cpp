#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <fstream>
#include <Windows.h>

#include <Enums.h>

#include "Map.h"
#include "Layers.h"
#include "Population.h"

Modes mode;
VisualizationTypes visualization_type;

std::shared_ptr<Map> map;
std::shared_ptr<Layers> layers;
std::shared_ptr<Population> population;

sf::Font font;
sf::Image icon;
sf::Sprite loading;
sf::Texture loading_texture;

std::fstream f_out, f_in;
std::string path_info, path_input, path_output, str_input;

std::vector<sf::Vector2f> pos;
std::vector<std::string> map_markup;

sf::RectangleShape rect;
sf::Text text[4], controls[3];
sf::Vector2i map_size, wall_size, pos_agent, pos_goal;

float radius_agent, radius_goal, max_speed, mutation_rate;
unsigned int width, height, directions_array_size, population_quantity, layers_quantity, auto_completion;
bool from_image, check_from_file, map_loaded, result_loaded, output_path_set, pause, show_controls, around, map_changed;

void initialize_variables(void)
{
	font.loadFromFile("Resource Files/Fonts/sans-serif.ttf");

	loading_texture.loadFromFile("Resource Files/Textures/loading.png");
	loading.setOrigin(100, 25);
	loading.setPosition(400, 400);
	loading.setTexture(loading_texture);

	icon.loadFromFile("icon.png");

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

	visualization_type = VisualizationTypes::WITH;

	max_speed = 5;
	auto_completion = 10;
	map_size.x = 80;
	map_size.y = 80;
	radius_agent = 4;
	radius_goal = 10;
	layers_quantity = 3;
	mutation_rate = 0.01f;
	population_quantity = 250;
	directions_array_size = 400;

	around = false;
	map_changed = false;

	controls[0].setPosition(685, 10);
	controls[0].setString(L"[Esc] - Exit");
	controls[1].setPosition(612, 10);
	controls[1].setString(L"[H] - Show controls");
	controls[2].setPosition(612, 10);
	controls[2].setString(L"[H] - Show controls");

	for (int i = 0; i < 3; i++)
	{
		controls[i].setFont(font);
		controls[i].setCharacterSize(20);
		controls[i].setOutlineThickness(3);
		controls[i].setOutlineColor(sf::Color::White);
		controls[i].setFillColor(sf::Color::Black);
	}

	rect.setFillColor(sf::Color::Blue);
	rect.setSize(sf::Vector2f(10, 10));

	text[0].setPosition(15, 10);
	text[0].setString(L"Reached the goal: ");
	text[1].setPosition(185, 10);
	text[1].setString(L"No");
	text[2].setPosition(15, 30);
	text[2].setString(L"Gen: ");
	text[3].setPosition(65, 30);
	text[3].setString(L"0");

	for (int i = 0; i < 4; i++)
	{
		text[i].setFont(font);
		text[i].setCharacterSize(20);
		text[i].setOutlineThickness(3);
		text[i].setOutlineColor(sf::Color::White);
		text[i].setFillColor(sf::Color::Black);
	}
}

void draw(sf::Event& event)
{
	if (event.type == event.KeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && map->pos_rects.size() > 0)
			map->pos_rects.pop_back();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && map->pos_additional_rewards.size() > 0)
			map->pos_additional_rewards.pop_back();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !around)
		{
			around = true;

			for (int x = 0; x < map_size.x; x++)
			{
				map->pos_rects.emplace_back(static_cast<float>(x * (width / map_size.x)), 0.0f);
				map->pos_rects.emplace_back(static_cast<float>(x * (width / map_size.x)), static_cast<float>(map_size.y * (height / map_size.y)) - (height / map_size.y));
			}

			for (int y = 0; y < map_size.y; y++)
			{
				map->pos_rects.emplace_back(0.0f, static_cast<float>(y * (width / map_size.y)));
				map->pos_rects.emplace_back(static_cast<float>(map_size.x * (width / map_size.x)) - (width / map_size.x), static_cast<float>(y)* (height / map_size.y));
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) radius_goal += 1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && radius_goal > 0) radius_goal -= 1.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) radius_agent += 1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && radius_agent > 0) radius_agent -= 1.0f;
	}

	if (event.type == event.MouseMoved)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if ([&]
				{
					for (auto& el : map->pos_rects)
						if (el == sf::Vector2f(static_cast<float>((event.mouseMove.x / (width / map_size.x)) * (width / map_size.x)), static_cast<float>((event.mouseMove.y / (height / map_size.y)) * (height / map_size.y))))
							return false;
						return true;
				}())
				map->pos_rects.emplace_back(static_cast<float>((event.mouseMove.x / (width / map_size.x)) * (width / map_size.x)), static_cast<float>((event.mouseMove.y / (height / map_size.y)) * (height / map_size.y)));
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if ([&]
				{
					for (auto& el : map->pos_additional_rewards)
						if (el == sf::Vector2f(static_cast<float>((event.mouseMove.x / (width / map_size.x)) * (width / map_size.x)), static_cast<float>((event.mouseMove.y / (height / map_size.y)) * (height / map_size.y))))
							return false;
						return true;
				}())
				map->pos_rects.emplace_back(static_cast<float>((event.mouseMove.x / (width / map_size.x)) * (width / map_size.x)), static_cast<float>((event.mouseMove.y / (height / map_size.y)) * (height / map_size.y)));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			map->pos_goal = sf::Vector2f(static_cast<float>(event.mouseMove.x / (width / map_size.x) * (width / map_size.x)), static_cast<float>(event.mouseMove.y / (height / map_size.y) * (height / map_size.y)));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			map->pos_agent = sf::Vector2f(static_cast<float>(event.mouseMove.x / (width / map_size.x) * (width / map_size.x)), static_cast<float>(event.mouseMove.y / (height / map_size.y) * (height / map_size.y)));
	}
}

void check()
{
	std::vector<sf::Vector2f> check;

	sf::RenderWindow window(sf::VideoMode(width, height), "Check");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && event.type == event.KeyPressed))
				window.close();

		window.clear(sf::Color::White);

		if (population->gen == 1)
			population->update();

		map->show(window);
		population->show(window);

		if (!population->reached_the_goal) check.emplace_back(population->agents[0].circle.getPosition());

		for (auto& el : check)
		{
			map->circle_agent.setPosition(el);
			window.draw(map->circle_agent);
		}

		window.draw(controls[0]);
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

			if (event.type == event.KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					f_out.open(path_input);
					window.draw(loading);
					window.display();

					f_out.clear();
					f_out << "map-size:;" << map_size.x << ';' << map_size.y << std::endl;
					for (int y = 0; y < map_size.y; y++)
					{
						for (int x = 0; x < map_size.x; x++)
						{
							if (x == map->pos_agent.x / (width / map_size.x) && y == map->pos_agent.y / (height / map_size.y))
								f_out << 'A';
							else if (x == map->pos_goal.x / (width / map_size.x) && y == map->pos_goal.y / (height / map_size.y))
								f_out << 'G';
							else if ([&x, &y]
								{
									for (auto& el : map->pos_rects)
										if (x == el.x / 10 && y == el.y / 10)
											return true;
										return false;
								}())
								f_out << 'W';
							else
								f_out << 'S';
							f_out << ';';
						}
						f_out << std::endl;
					}
					f_out << "agent-radius:;" << radius_agent << std::endl;
					f_out << "goal-radius:;" << radius_goal << std::endl;
					f_out.close();
					window.close();
				}

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
						controls[1].setString(L"[LShift] - Move goal\n[RShift] - Move agent\n[Up][Down] - Resize goal\n[left][right] - Resize agent\n[Tab] - Fill around\n[LMB] - Draw map\n[RMB] - Draw reward\n[RCtrl] - Erase map\n[RCtrl] - Erase reward\n[Esc] - Exit without saving\n[Enter] - save&exit\n[H] - Hide controls");
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					window.close();
			}

			draw(event);
		}

		window.clear(sf::Color::White);

		map->show(window);

		window.draw(controls[1]);

		window.display();
	}
}

void load_map_from_file()
{
	str_input.clear();

	for (int i = static_cast<int>(path_input.length()) - 1; i >= 0; i--)
		if (path_input[i] != '.') str_input += path_input[i];
		else break;

	if (str_input == "pmb" || str_input == "gnp" || str_input == "agt" || str_input == "gpj" ||
		str_input == "fig" || str_input == "dsp" || str_input == "rdh" || str_input == "cip") from_image = true;
	else from_image = false;

	if (from_image)
	{
		sf::Image map_image;
		map_image.loadFromFile(path_input);
		map_size.x = static_cast<int>(map_image.getSize().x) / 10;
		map_size.y = static_cast<int>(map_image.getSize().y) / 10;
		map.reset(new Map);

		for (unsigned int y = 0; y < static_cast<unsigned int>(map_size.y); y++)
		{
			map_markup[y].resize(map_size.x);
			for (unsigned int x = 0; x < static_cast<unsigned int>(map_size.x); x++)
			{
				if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y* (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Black)
					map_markup[y][x] = 'A';
				else if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y* (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Red)
					map_markup[y][x] = 'G';
				else if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y* (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Blue)
					map_markup[y][x] = 'W';
				else if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y* (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Yellow)
					map_markup[y][x] = 'B';
				else
					map_markup[y][x] = 'S';
			}
		}

		map->from_file();

		if (path_input != "")
			map_loaded = true;
	}
	else
	{
		f_in.open(path_input);
		if (f_in.is_open())
		{
			map.reset(new Map);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input, ';');
			map_size.x = std::stoi(str_input);

			std::getline(f_in, str_input);
			map_size.y = std::stoi(str_input);

			map_markup.resize(static_cast<unsigned int>(map_size.y));
			for (auto& el : map_markup)
				el.resize(static_cast<unsigned int>(map_size.x));

			for (int y = 0; y < map_size.y; y++)
			{
				std::getline(f_in, str_input);
				str_input.erase(std::remove(str_input.begin(), str_input.end(), ';'), str_input.end());
				map_markup[y] = str_input;
			}

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			radius_agent = std::stof(str_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			radius_goal = std::stof(str_input);

			map->from_file();
			f_in.close();
			map_loaded = true;
		}
		else
		{
			// if file is not opened
			exit(0);
		}
	}
}

void load_result_from_file()
{
	f_in.open(path_output);

	if (f_in.is_open())
	{
		std::getline(f_in, str_input, ';');
		std::getline(f_in, str_input);
		max_speed = std::stof(str_input);

		std::getline(f_in, str_input, ';');
		std::getline(f_in, str_input);
		directions_array_size = std::stoi(str_input);

		population_quantity = 1;
		population.reset(new Population);

		for (auto& el : population->agents[0].brain.directions)
		{
			std::getline(f_in, str_input, ';');
			el.x = std::stof(str_input);

			std::getline(f_in, str_input, '\n');
			el.y = std::stof(str_input);
		}
		f_in.close();
		result_loaded = true;
	}
	else
	{
		// if file is not opened
		exit(0);
	}
}

void with_visualization()
{
	layers.reset(new Layers);

	sf::RenderWindow window(sf::VideoMode(width, height), "Learn");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && event.type == event.KeyPressed))
				window.close();

			draw(event);

			if (event.type == event.KeyPressed || (layers->populations[layers->best_population].after_reach > auto_completion))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) ||
					(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !around) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					map_changed = true;

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
					sf::Mouse::isButtonPressed(sf::Mouse::Right) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
					map_changed = true;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
					for (auto& el : layers->populations)
						el.min_step = directions_array_size;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || (layers->populations[layers->best_population].after_reach > auto_completion))
				{
					window.draw(loading);
					window.display();

					f_out.open(path_output);
					f_out.clear();
					f_out << "max-speed:;" << max_speed << std::endl;
					f_out << "directions-array-size:;" << layers->populations[layers->best_population].min_step << std::endl;
					for (unsigned int i = 0; i < layers->populations[layers->best_population].min_step; ++i)
						f_out << layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].x << ';' << layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].y << std::endl;
					f_out.close();
					if (!map_changed)
						window.close();

					if (map_changed)
					{
						f_out.open(path_output);
						f_out.clear();
						f_out << "map-size:;" << map_size.x << ';' << map_size.y << std::endl;
						for (int y = 0; y < map_size.y; y++)
						{
							for (int x = 0; x < map_size.x; x++)
							{
								if (x == map->pos_agent.x / 10 && y == map->pos_agent.y / 10)
									f_out << 'A';
								else if (x == map->pos_goal.x / 10 && y == map->pos_goal.y / 10)
									f_out << 'G';
								else if ([&x, &y]
									{
										for (auto& el : map->pos_rects)
											if (x == el.x / 10 && y == el.y / 10)
												return true;
											return false;
									}())
									f_out << 'W';
								else
									f_out << 'S';
								f_out << ';';
							}
							f_out << std::endl;
						}
						f_out << "agent-radius:;" << radius_agent << std::endl;
						f_out << "goal-radius:;" << radius_goal << std::endl;
						f_out.close();
						window.close();
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) pause = !pause;

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
						controls[2].setString(L"[LShift] - Move goal\n[RShift] - Move agent\n[Up][Down] - Resize goal\n[left][right] - Resize agent\n[Tab] - Fill around\n[LMB] - Draw map\n[RMB] - Draw rewards\n[RCtrl] - Erase map\n[RCtrl] - Erase reward\n[R] - Reset minimum steps\n[Esc] - Exit without saving\n[Enter] - save&exit\n[H] - Hide controls");
					}
				}
			}
		}

		window.clear(sf::Color::White);

		if (!pause) layers->update();

		map->show(window);
		layers->show(window);

		if (layers->populations[layers->best_population].reached_the_goal) text[1].setString(L"Yes");
		else text[1].setString(L"No");

		std::ostringstream str;
		str << layers->populations[layers->best_population].gen;
		text[3].setString(str.str());

		for (unsigned int i = 0; i < 4; i++)
			window.draw(text[i]);
		window.draw(controls[2]);

		window.display();
	}
}

void without_visualization()
{
	layers.reset(new Layers);
	bool calculating = true;

	while (calculating)
	{
		if (layers->populations[layers->best_population].after_reach > auto_completion)
		{
			f_out.open(path_output);
			f_out.clear();
			f_out << "max-speed:;" << max_speed << std::endl;
			f_out << "directions-array-size:;" << layers->populations[layers->best_population].min_step << std::endl;
			for (unsigned int i = 0; i < layers->populations[layers->best_population].min_step; ++i)
				f_out << layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].x << " ; " 
					<< layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].y << std::endl;
			f_out.close();
			calculating = false;
		}

		layers->update();
	}
}

void load_parameters_from_info_file()
{
	path_info = "C:\\Users\\vhd-r\\Documents\\GitHub\\mlvl\\Source Files\\Windows x64\\Algorithms\\NEAT\\NEAT\\Resource Files\\Data\\info-check.csv";
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
			if (str_input == "WITH") visualization_type = VisualizationTypes::WITH;
			else if (str_input == "WITHOUT") visualization_type = VisualizationTypes::WITHOUT;

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			directions_array_size = std::stoi(str_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			population_quantity = std::stoi(str_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			layers_quantity = std::stoi(str_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			auto_completion = std::stoi(str_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			max_speed = std::stof(str_input);

			std::getline(f_in, str_input, ';');
			std::getline(f_in, str_input);
			mutation_rate = std::stof(str_input);
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
			with_visualization();
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
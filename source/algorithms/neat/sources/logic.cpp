#include <NEAT/Header Files/NEAT_Logic.h>

void neat::draw(sf::Event &event)
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
				map->pos_rects.emplace_back(static_cast<float>(map_size.x * (width / map_size.x)) - (width / map_size.x), static_cast<float>(y) * (height / map_size.y));
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) goal_radius += 1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && goal_radius > 0) goal_radius -= 1.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) agent_radius += 1.0f;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && agent_radius > 0) agent_radius -= 1.0f;
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
				map->pos_rects.emplace_back(static_cast<float>((event.mouseMove.x / (width / map_size.x))* (width / map_size.x)), static_cast<float>((event.mouseMove.y / (height / map_size.y))* (height / map_size.y)));
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
				map->pos_rects.emplace_back(static_cast<float>((event.mouseMove.x / (width / map_size.x))* (width / map_size.x)), static_cast<float>((event.mouseMove.y / (height / map_size.y))* (height / map_size.y)));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			map->pos_goal = sf::Vector2f(static_cast<float>(event.mouseMove.x / (width / map_size.x) * (width / map_size.x)), static_cast<float>(event.mouseMove.y / (height / map_size.y) * (height / map_size.y)));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			map->pos_agent = sf::Vector2f(static_cast<float>(event.mouseMove.x / (width / map_size.x) * (width / map_size.x)), static_cast<float>(event.mouseMove.y / (height / map_size.y) * (height / map_size.y)));
	}
}

void neat::check_2d()
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

void neat::check_3d()
{
	char buffer[260];
	GetCurrentDirectory(sizeof(buffer), buffer);
	std::string str = buffer;
	str.erase(str.size() - 14);
	str += "data\\info.csv";

	fout.open(str);
	if (fout.is_open())
	{
		fout.clear();
		fout << path_input << std::endl;
		fout << path_output << std::endl;
		fout << "NEAT" << std::endl;
		fout << "CHECK" << std::endl;
		fout.close();
	}
	else
	{
		std::string message = "Error opening file: \"" + str + "\"";
		System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}

	str = buffer;
	str = "\"" + str;
	str.erase(str.size() - 14);
	str += "bin\\win64\\unity\\UNITY C#.exe\"";

	system(str.c_str());
}

void neat::create_new_map_2d()
{
	map_size.x = 80;
	map_size.y = 80;
	map.reset(new Map());

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
					System::Windows::Forms::SaveFileDialog^ save_file_dialog = gcnew System::Windows::Forms::SaveFileDialog();
					save_file_dialog->Title = "Save map";
					save_file_dialog->DefaultExt = "csv";
					save_file_dialog->AddExtension = true;
					save_file_dialog->Filter = "CSV|*.csv";
					save_file_dialog->FileName = "new_map";
					if (save_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
						path_input = static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(save_file_dialog->InitialDirectory + save_file_dialog->FileName).ToPointer());

					if (path_input != "")
					{
						System::IO::StreamWriter^ new_file = gcnew System::IO::StreamWriter(gcnew System::String(path_input.c_str()));
						new_file->Close();
					}
					else
					{
						std::string message = "Error: Please, choose file to save map";
						System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
					}

					fout.open(path_input);
					if (fout.is_open())
					{
						window.draw(loading);
						window.display();

						fout.clear();
						fout << "map-size:;" << map_size.x << ';' << map_size.y << std::endl;
						for (int y = 0; y < map_size.y; y++)
						{
							for (int x = 0; x < map_size.x; x++)
							{
								if (x == map->pos_agent.x / (width / map_size.x) && y == map->pos_agent.y / (height / map_size.y))
									fout << 'A';
								else if (x == map->pos_goal.x / (width / map_size.x) && y == map->pos_goal.y / (height / map_size.y))
									fout << 'G';
								else if ([&x, &y]
								{
									for (auto& el : map->pos_rects)
										if (x == el.x / 10 && y == el.y / 10)
											return true;
										return false;
								}())
									fout << 'W';
								else
									fout << 'S';
								fout << ';';
							}
							fout << std::endl;
						}
						fout << "agent-radius:;" << agent_radius << std::endl;
						fout << "goal-radius:;" << goal_radius << std::endl;
						fout.close();
						window.close();
					}
					else
					{
						std::string message = "Error opening file: \"" + path + "\"";
						System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
				{
					show_controls = !show_controls;
					if (show_controls)
					{
						neat::controls[1].setPosition(612, 10);
						neat::controls[1].setString(L"[H] - Show controls");
					}
					else
					{
						neat::controls[1].setPosition(552, 10);
						neat::controls[1].setString(L"[LShift] - Move goal\n[RShift] - Move agent\n[Up][Down] - Resize goal\n[left][right] - Resize agent\n[Tab] - Fill around\n[LMB] - Draw map\n[RMB] - Draw reward\n[RCtrl] - Erase map\n[RCtrl] - Erase reward\n[Esc] - Exit without saving\n[Enter] - save&exit\n[H] - Hide controls");
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

void neat::create_new_map_3d()
{
	System::Windows::Forms::MessageBox::Show("Open \"NEAT Map Creator\" as Unity Project and press start when done creating new map. Then load it from file", "NEAT Map Creator", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
	char buffer[260];
	GetCurrentDirectory(sizeof(buffer), buffer);
	std::string str1 = "explorer.exe ";
	std::string str2 = str1 + buffer;
	str2.erase(str2.size() - 14);
	str2 += "unity";
	system(str2.c_str());
}

void neat::set_result_file_path()
{
	System::Windows::Forms::SaveFileDialog^ save_file_dialog = gcnew System::Windows::Forms::SaveFileDialog();
	save_file_dialog->AddExtension = true;
	save_file_dialog->Filter = "CSV|*.csv";
	save_file_dialog->FileName = "new_result";
	save_file_dialog->Title = "Set result file";
	if (save_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		path_output = static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(save_file_dialog->InitialDirectory + save_file_dialog->FileName).ToPointer());

	if (path_output != "")
	{
		System::IO::StreamWriter^ new_file = gcnew System::IO::StreamWriter(gcnew System::String(path_output.c_str()));
		new_file->Close();
	}
	else
	{
		std::string message = "Error: Please, choose file to save map";
		System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}

	if (path_output != "")
		output_path_set = true;
}

void neat::load_map_from_file_2d()
{
	System::Windows::Forms::OpenFileDialog^ open_file_dialog = gcnew System::Windows::Forms::OpenFileDialog();
	open_file_dialog->Title = "Load map";
	open_file_dialog->AddExtension = true;
	open_file_dialog->Filter = "CSV|*.csv|BMP|*.bmp|PNG|*.png|TGA|*.tga|JPG|*.jpg|GIF|*.gif|PSD|*.psd|HDR|*.hdr|PIC|*.pic";
	if (open_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		path_input = static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(open_file_dialog->InitialDirectory + open_file_dialog->FileName).ToPointer());
	
	std::string str;
	for (int i = static_cast<int>(path_input.length()) - 1; i >= 0; i--)
		if (path_input[i] != '.') str += path_input[i];
		else break;

	if (str == "pmb" || str == "gnp" || str == "agt" || str == "gpj" ||
		str == "fig" || str == "dsp" || str == "rdh" || str == "cip") from_image = true;
	else from_image = false;

	str.clear();

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
				if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y * (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Black)
					map_markup[y][x] = 'A';
				else if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y * (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Red)
					map_markup[y][x] = 'G';
				else if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y * (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Blue)
					map_markup[y][x] = 'W';
				else if (map_image.getPixel(x * (map_image.getSize().x / static_cast<unsigned int>(map_size.x)), y * (map_image.getSize().y / static_cast<unsigned int>(map_size.y))) == sf::Color::Yellow)
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
		fin.open(path_input);
		if (fin.is_open())
		{
			map_size.x = 80;
			map_size.y = 80;
			map.reset(new Map);

			char ch = '\0';

			while (ch != ';') fin.get(ch);
			fin >> map_size.x;
			fin.get(ch);
			fin >> map_size.y;
			fin.get(ch);

			std::string line;
			for (int y = 0; y < map_size.y; y++)
			{
				std::getline(fin, line);
				line.erase(std::remove(line.begin(), line.end(), ';'), line.end());
				map_markup[y] = line;
			}

			while (ch != ';') fin.get(ch);
			fin >> agent_radius;
			fin.get(ch);

			while (ch != ';') fin.get(ch);
			fin >> goal_radius;

			map->from_file();
			fin.close();
			map_loaded = true;
		}
		else
		{
			std::string message = "Error opening file \"" + path_input + "\"";
			System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		}
	}
}

void neat::load_map_from_file_3d()
{
	System::Windows::Forms::OpenFileDialog^ open_file_dialog = gcnew System::Windows::Forms::OpenFileDialog();
	open_file_dialog->Title = "Load map";
	open_file_dialog->AddExtension = true;
	open_file_dialog->Filter = "CSV|*.csv";
	if (open_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		path_input = static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(open_file_dialog->InitialDirectory + open_file_dialog->FileName).ToPointer());
	
	if (path_input != "")
		map_loaded = true;
}

void neat::load_result_from_file_2d()
{
	System::Windows::Forms::OpenFileDialog^ open_file_dialog = gcnew System::Windows::Forms::OpenFileDialog();
	open_file_dialog->Title = "Load result";
	open_file_dialog->AddExtension = true;
	open_file_dialog->Filter = "CSV|*.csv";
	if (open_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		path_output = static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(open_file_dialog->InitialDirectory + open_file_dialog->FileName).ToPointer());
	
	fin.open(path_output);
	if (fin.is_open())
	{
		char ch = '\0';

		while (ch != ';') fin.get(ch);
		fin >> max_speed;
		fin.get(ch);
		while (ch != ';') fin.get(ch);
		fin >> directions_array_size;
		fin.get(ch);

		population_quantity = 1;
		population.reset(new Population());

		std::string line;
		for (auto& el : population->agents[0].brain.directions)
		{
			std::getline(fin, line, ';');
			std::replace(line.begin(), line.end(), '.', ',');
			el.x = System::Convert::ToSingle(gcnew System::String(line.c_str()));

			std::getline(fin, line, '\n');
			std::replace(line.begin(), line.end(), '.', ',');
			el.y = System::Convert::ToSingle(gcnew System::String(line.c_str()));
		}
		fin.close();

		result_loaded = true;
	}
	else
	{
		std::string message = "Error opening file \"" + path + "\"";
		System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}
}

void neat::load_result_from_file_3d()
{
	System::Windows::Forms::OpenFileDialog^ open_file_dialog = gcnew System::Windows::Forms::OpenFileDialog();
	open_file_dialog->Title = "Load result";
	open_file_dialog->AddExtension = true;
	open_file_dialog->Filter = "CSV|*.csv";
	if (open_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		path_output = static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(open_file_dialog->InitialDirectory + open_file_dialog->FileName).ToPointer());
	
	if (path_output != "")
		result_loaded = true;
}

void neat::with_visualization_2d()
{
	layers.reset(new Layers());

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

			if (event.type == event.KeyPressed || (layers->populations[layers->best_population].after_reach > auto_exit))
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

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || (layers->populations[layers->best_population].after_reach > auto_exit))
				{
					window.draw(loading);
					window.display();

					fout.open(path_output);
					if (fout.is_open())
					{
						fout.clear();
						fout << "max-speed:;" << max_speed << std::endl;
						fout << "directions-array-size:;" << layers->populations[layers->best_population].min_step << std::endl;
						for (int i = 0; i < layers->populations[layers->best_population].min_step; ++i)
							fout << layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].x << ';' << layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].y << std::endl;
						fout.close();
						if (!map_changed)
							window.close();
					}
					else
					{
						std::string message = "Error opening file: \"" + path_output + "\"";
						System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
					}

					if (map_changed)
					{
						System::Windows::Forms::SaveFileDialog^ save_file_dialog = gcnew System::Windows::Forms::SaveFileDialog();
						save_file_dialog->DefaultExt = "csv";
						save_file_dialog->AddExtension = true;
						save_file_dialog->Filter = "CSV|*.csv";
						save_file_dialog->FileName = "new_map";
						save_file_dialog->Title = "Save map";
						if (save_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
							path = static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(save_file_dialog->InitialDirectory + save_file_dialog->FileName).ToPointer());

						if (path != "")
						{
							System::IO::StreamWriter^ new_file = gcnew System::IO::StreamWriter(gcnew System::String(path.c_str()));
							new_file->Close();
						}
						else
						{
							std::string message = "Error: Please, choose file to save map";
							System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
						}

						fout.open(path);
						if (fout.is_open())
						{
							fout.clear();
							fout << "map-size:;" << map_size.x << ';' << map_size.y << std::endl;
							for (int y = 0; y < map_size.y; y++)
							{
								for (int x = 0; x < map_size.x; x++)
								{
									if (x == map->pos_agent.x / 10 && y == map->pos_agent.y / 10)
										fout << 'A';
									else if (x == map->pos_goal.x / 10 && y == map->pos_goal.y / 10)
										fout << 'G';
									else if ([&x, &y]
										{
											for (auto& el : map->pos_rects)
												if (x == el.x / 10 && y == el.y / 10)
													return true;
												return false;
										}())
										fout << 'W';
									else
										fout << 'S';
									fout << ';';
								}
								fout << std::endl;
							}
							fout << "agent-radius:;" << agent_radius << std::endl;
							fout << "goal-radius:;" << goal_radius << std::endl;
							fout.close();
							window.close();
						}
						else
						{
							std::string message = "Error opening file: \"" + path + "\"";
							System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
						}
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) pause = !pause;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
				{
					show_controls = !show_controls;
					if (show_controls)
					{
						neat::controls[2].setPosition(612, 10);
						neat::controls[2].setString(L"[H] - Show controls");
					}
					else
					{
						neat::controls[2].setPosition(552, 10);
						neat::controls[2].setString(L"[LShift] - Move goal\n[RShift] - Move agent\n[Up][Down] - Resize goal\n[left][right] - Resize agent\n[Tab] - Fill around\n[LMB] - Draw map\n[RMB] - Draw rewards\n[RCtrl] - Erase map\n[RCtrl] - Erase reward\n[R] - Reset minimum steps\n[Esc] - Exit without saving\n[Enter] - save&exit\n[H] - Hide controls");
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

		for (int i = 0; i < 4; i++)
			window.draw(text[i]);
		window.draw(controls[2]);
		window.display();
	}
}

void neat::with_visualization_3d()
{
	char buffer[260];
	GetCurrentDirectory(sizeof(buffer), buffer);
	std::string str = buffer;
	str.erase(str.size() - 14);
	str += "data\\info.csv";

	fout.open(str);
	if (fout.is_open())
	{
		fout.clear();
		fout << path_input << std::endl;
		fout << path_output << std::endl;
		fout << "NEAT" << std::endl;
		fout << "LEARN" << std::endl;
		fout << "visualization:;" << visualization_type << std::endl;
		fout << "direction-array-size:;" << directions_array_size << std::endl;
		fout << "population-quantity:;" << population_quantity << std::endl;
		fout << "layers-quantity:;" << layers_quantity << std::endl;
		fout << "auto-completion:;" << auto_exit << std::endl;
		fout << "max-speed:;" << max_speed << std::endl;
		fout << "mutation-rate:;" << static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(mutation_rate.ToString()).ToPointer()) << std::endl;
		fout.close();
	}
	else
	{
		std::string message = "Error opening file: \"" + str + "\"";
		System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}

	str = buffer;
	str = "\"" + str;
	str.erase(str.size() - 14);
	str += "bin\\win64\\unity\\UNITY C#.exe\"";

	system(str.c_str());
}

void neat::without_visualization_2d()
{
	layers.reset(new Layers());
	bool calculating = true;

	while (calculating)
	{
		if (layers->populations[layers->best_population].after_reach > auto_exit)
		{
			fout.open(path_output);
			if (fout.is_open())
			{
				fout.clear();
				fout << "max-speed:;" << max_speed << std::endl;
				fout << "directions-array-size:;" << layers->populations[layers->best_population].min_step << std::endl;
				for (int i = 0; i < layers->populations[layers->best_population].min_step; ++i)
					fout << layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].x << " ; " << layers->populations[layers->best_population].agents[layers->populations[layers->best_population].best_agent].brain.directions[i].y << std::endl;
				fout.close();
				calculating = false;
			}
			else
			{
				std::string message = "Error opening file: \"" + path_output + "\"";
				System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
			}
		}

		layers->update();
	}
}

void neat::without_visualization_3d()
{
	char buffer[260];
	GetCurrentDirectory(sizeof(buffer), buffer);
	std::string str = buffer;
	str.erase(str.size() - 14);
	str += "data\\info.csv";

	fout.open(str);
	if (fout.is_open())
	{
		fout.clear();
		fout << path_input << std::endl;
		fout << path_output << std::endl;
		fout << "NEAT" << std::endl;
		fout << "LEARN" << std::endl;
		fout << "visualization:;" << visualization_type << std::endl;
		fout << "direction-array-size:;" << directions_array_size << std::endl;
		fout << "population-quantity:;" << population_quantity << std::endl;
		fout << "layers-quantity:;" << layers_quantity << std::endl;
		fout << "auto-completion:;" << auto_exit << std::endl;
		fout << "max-speed:;" << max_speed << std::endl;
		fout << "mutation-rate:;" << static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(mutation_rate.ToString()).ToPointer()) << std::endl;
		fout.close();
	}
	else
	{
		std::string message = "Error opening file: \"" + str + "\"";
		System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}

	str = buffer;
	str = "\"" + str;
	str.erase(str.size() - 14);
	str += "bin\\win64\\unity\\UNITY C#.exe\"";

	system(str.c_str());
}
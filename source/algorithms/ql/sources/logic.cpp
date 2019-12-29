#include <QL/Header Files/QL_Logic.h>

void ql::draw(sf::Event& event)
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.mouseMove.x < (height / map_size.x) * map_size.x && event.mouseMove.y < (width / map_size.y) * map_size.y)
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && event.mouseMove.x < (height / map_size.x) * map_size.x && event.mouseMove.y < (width / map_size.y) * map_size.y)
		{
			map->map_markup[static_cast<int>(map->goal.getPosition().y) / (height / map_size.y)][static_cast<int>(map->goal.getPosition().x) / (width / map_size.x)] = 'S';
			map->goal.setPosition(static_cast<float>(event.mouseMove.x / (width / map_size.x) * (width / map_size.x)), static_cast<float>(event.mouseMove.y / (height / map_size.y) * (height / map_size.y)));
			map->update();
		}
	}
}

void ql::check_2d()
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
						if (moves.size() > map_size.x * map_size.y)
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

void ql::check_3d()
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
		fout << "QL" << std::endl;
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

void ql::create_new_map_2d()
{
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

			draw(event);

			if (event.type == event.KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					System::Windows::Forms::SaveFileDialog^ save_file_dialog = gcnew System::Windows::Forms::SaveFileDialog();
					save_file_dialog->DefaultExt = "csv";
					save_file_dialog->AddExtension = true;
					save_file_dialog->Filter = "CSV|*.csv";
					save_file_dialog->FileName = "new_map";
					save_file_dialog->Title = "Save map";
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

					window.draw(loading);
					window.display();

					map->save();

					fout.open(path_input);
					if (fout.is_open())
					{
						fout.clear();
						fout << "map-size:;" << map_size.x << ';' << map_size.y << std::endl;

						for (int y = 0; y < map_size.y; y++)
						{
							for (int x = 0; x < map_size.x; x++)
								fout << map->map_markup[y][x] << ';';
							fout << std::endl;
						}

						fout.close();
						window.close();
						map_loaded = true;
					}
					else
					{
						std::string message = "Error opening file: \"" + path_input + "\"";
						System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
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

void ql::create_new_map_3d()
{
	System::Windows::Forms::MessageBox::Show("Open \"QL Map Creator\" as Unity Project and press start when done creating new map. Then load it from file", "QL Map Creator", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Information);
	char buffer[260];
	GetCurrentDirectory(sizeof(buffer), buffer);
	std::string str1 = "explorer.exe ";
	std::string str2 = str1 + buffer;
	str2.erase(str2.size() - 14);
	str2 += "unity";
	system(str2.c_str());
}

void ql::set_result_file_path()
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

void ql::load_map_from_file_2d()
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
		map.reset(new Map());

		fin.open(path_input);
		if (fin.is_open())
		{
			char ch = '\0';
			while (ch != ';') fin.get(ch);

			fin >> map_size.x;
			fin.get();

			fin >> map_size.y;
			fin.get();

			map->map_markup.resize(map_size.y);
			for (int y = 0; y < map_size.y; y++)
				map->map_markup[y].resize(map_size.x);

			std::string line;
			for (int y = 0; y < map_size.y; y++)
			{
				std::getline(fin, line);
				line.erase(std::remove(line.begin(), line.end(), ';'), line.end());
				for (int x = 0; x < map_size.x; x++)
					map->map_markup[y][x] = line[x];
			}

			fin.close();

			map_loaded = true;
		}
		else
		{
			std::string message = "Error opening file \"" + path_input + "\"";
			System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		}
	}

	map->update();
	goal_reward = static_cast<long long>(std::pow(map_size.x * map_size.y, 3));
	table.reset(new Table());
}

void ql::load_map_from_file_3d()
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

void ql::load_result_from_file_2d()
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
		std::string line;
		for (int y = 0; y < map_size.x * map_size.y; y++)
			for (int x = 0; x < map_size.x * map_size.y; x++)
			{
				std::getline(fin, line, ';');
				line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
				table->Q[y][x] = System::Convert::ToInt64(gcnew System::String(line.c_str()), 10);
			}
		fin.close();
	}
	else
	{
		std::string message = "Error opening file \"" + path_output + "\"";
		System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}

	result_loaded = true;
}

void ql::load_result_from_file_3d()
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

void ql::with_visualization_2d()
{
	agent.reset(new Agent());

	sf::RenderWindow window(sf::VideoMode(width, height), "Learn");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	int repetitions_k = 0, initials_k = 0;

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

		for (int i = 0; i < 4; i++)
			window.draw(text[i]);

		window.draw(controls[0]);

		window.display();

		if (repetitions_k > repetitions - 1)
		{
			fout.open(path_output);
			if (fout.is_open())
			{
				fout.clear();
				window.draw(loading);
				window.display();

				for (int y = 0; y < map_size.x * map_size.y; y++)
				{
					for (int x = 0; x < map_size.x * map_size.y; x++)
						fout << table->Q[y][x] << ';';
					fout << std::endl;
				}
				fout.close();

				window.close();
			}
			else
			{
				std::string message = "Error opening file: \"" + path_output + "\"";
				System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
			}
		}
	}
}

void ql::with_visualization_3d()
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
		fout << "QL" << std::endl;
		fout << "LEARN" << std::endl;
		fout << "gamma:;" << gamma << std::endl;
		fout << "iterations:;" << iterations << std::endl;
		fout << "repetitions:;" << repetitions << std::endl;
		fout << "visualization:;" << visualization_type << std::endl;
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

void ql::without_visualization_2d()
{
	table.reset(new Table());
	agent.reset(new Agent());

	for (int i = 0; i < repetitions; i++)
		for (auto& el : initials)
			table->episode(el);

	fout.open(path_input);
	if (fout.is_open())
	{
		fout.clear();
		for (int y = 0; y < map_size.x * map_size.y; y++)
		{
			for (int x = 0; x < map_size.x * map_size.y; x++)
				fout << table->Q[y][x] << ';';
			fout << std::endl;
		}
		fout.close();
	}
	else
	{
		std::string message = "Error opening file: \"" + path_output + "\"";
		System::Windows::Forms::MessageBox::Show(gcnew System::String(message.c_str()), "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
	}
}

void ql::without_visualization_3d()
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
		fout << "QL" << std::endl;
		fout << "LEARN" << std::endl;
		fout << "gamma:;" << gamma << std::endl;
		fout << "iterations:;" << iterations << std::endl;
		fout << "repetitions:;" << repetitions << std::endl;
		fout << "visualization:;" << visualization_type << std::endl;
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
}
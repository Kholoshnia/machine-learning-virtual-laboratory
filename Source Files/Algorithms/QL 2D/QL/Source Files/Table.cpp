#include "Table.h"

Table::Table()
{
	R.resize(map_size.y * map_size.y);
	for (auto& el : R)
		el.resize(map_size.y * map_size.y);

	Q.resize(map_size.y * map_size.y);
	for (auto& el : Q)
		el.resize(map_size.y * map_size.y);

	for (unsigned int state_y = 0; state_y < map_size.y * map_size.y; state_y++)
		for (unsigned int action_x = 0; action_x < map_size.x * map_size.x; action_x++)
			Q[state_y][action_x] = 0;

	srand(static_cast<unsigned int>(time(NULL)));

	for (int state_y = 0; state_y < map_size.y * map_size.y; state_y++)
		for (int action_x = 0; action_x < map_size.x * map_size.x; action_x++)
			R[state_y][action_x] = -1;

	for (int y = 0; y < map_size.y; y++)
	{
		for (int x = 0; x < map_size.x; x++)
		{
			if (x - 1 >= 0)
				if (map->map_markup[y][x - 1] == 'S')
					R[map_size.y * y + x][map_size.y * y + x - 1] = 0;
				else if (map->map_markup[y][x - 1] == 'G')
					R[map_size.y * y + x][map_size.y * y + x - 1] = goal_reward;

			if (x + 1 < map_size.x)
				if (map->map_markup[y][x + 1] == 'S')
					R[map_size.y * y + x][map_size.y * y + x + 1] = 0;
				else if (map->map_markup[y][x + 1] == 'G')
					R[map_size.y * y + x][map_size.y * y + x + 1] = goal_reward;

			if (y - 1 >= 0)
				if (map->map_markup[y - 1][x] == 'S')
					R[map_size.y * y + x][map_size.y * (y - 1) + x] = 0;
				else if (map->map_markup[y - 1][x] == 'G')
					R[map_size.y * y + x][map_size.y * (y - 1) + x] = goal_reward;

			if (y + 1 < map_size.y)
				if (map->map_markup[y + 1][x] == 'S')
					R[map_size.y * y + x][map_size.y * (y + 1) + x] = 0;
				else if (map->map_markup[y + 1][x] == 'G')
					R[map_size.y * y + x][map_size.y * (y + 1) + x] = goal_reward;

			if (map->map_markup[y][x] == 'G')
			{
				R[map_size.y * y + x][map_size.y * y + x] = goal_reward;
				goal_state = map_size.y * y + x;
			}
		}
	}
}

void Table::choose_an_action()
{
	int possible_action = get_random_action(0, map_size.x * map_size.x - 1);

	if (R[state][possible_action] >= 0)
	{
		Q[state][possible_action] = R[state][possible_action] + static_cast<long long>(gamma_parameter * maximum(possible_action, false));
		state = possible_action;
	}

	if (visualization_type == VisualizationTypes::STATES) agent->update(state);
}

void Table::episode(int init_state)
{
	state = init_state;
	if (state != goal_state)
		do
		{
			choose_an_action();
		} while (state == goal_state);
	for (unsigned int i = 0; i < iterations; i++)
		choose_an_action();

	if (visualization_type == VisualizationTypes::ITERATIONS) agent->update(init_state);
}

long long Table::maximum(int now_state, bool return_index)
{
	int best_action = 0;
	long long temp_max_q = 0;

	for (unsigned int i = 0; i < map_size.x * map_size.y; ++i)
		if (Q[now_state][i] > temp_max_q)
		{
			temp_max_q = Q[now_state][i];
			best_action = i;
		}

	if (return_index == true) return best_action;
	else return Q[now_state][best_action];
}

int Table::get_random_action(int lower_bound, int upper_bound)
{
	int act = 0, k = 0;
	bool choice_is_valid = false;

	do
	{
        act = rand() % (lower_bound + upper_bound) - lower_bound;
		if (R[state][act] > -1) choice_is_valid = true;
        if (++k > map_size.x * map_size.y) break;
	} while (choice_is_valid == false);

	return act;
}

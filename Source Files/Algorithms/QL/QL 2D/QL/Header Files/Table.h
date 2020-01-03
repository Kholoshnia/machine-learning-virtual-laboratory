#pragma once

#include <SFML/Graphics.hpp>

#include <ctime>
#include <vector>

#include "Enums.h"

#include "Map.h"
#include "Agent.h"

extern std::shared_ptr<Map> map;
extern std::shared_ptr<Agent> agent;

extern float gamma_parameter;
extern long long int goal_reward;
extern unsigned int iterations, goal_state;

extern VisualizationTypes visualization_type;

class Table
{
public:
	int state, action;
	std::vector<std::vector<long long int>> R;
	std::vector<std::vector<long long int>> Q;

	Table();
	void choose_an_action();
	void episode(int init_state);
	long long maximum(int now_state, bool return_index);
	int get_random_action(int lower_bound, int upper_bound);
};

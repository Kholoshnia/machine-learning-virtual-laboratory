#pragma once

#include <SFML/Graphics.hpp>

#include <ctime>
#include <vector>

#include <Enums.h>

#include <QL/Header Files/QL_Map.h>
#include <QL/Header Files/QL_Agent.h>

namespace ql
{
	extern std::shared_ptr<Map> map;
	extern std::shared_ptr<Agent> agent;

	extern float gamma;
	extern long long int goal_reward;
	extern int iterations, goal_state;

	extern VisualizationTypes visualization_type;

	struct Table
	{
		int state, action;
		std::vector<std::vector<long long int>> R;
		std::vector<std::vector<long long int>> Q;

		Table();
		void choose_an_action();
		void episode(int init_state);
		long long maximum(int now_state, bool return_index);
		int get_random_action(int lower_bound, int upper_bound);
	};
}
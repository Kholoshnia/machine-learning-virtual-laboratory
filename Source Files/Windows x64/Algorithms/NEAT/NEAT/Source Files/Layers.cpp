#include "Layers.h"

Layers::Layers() : best_population(0)
{
	populations.resize(layers_quantity);
	threads.resize(layers_quantity);
	for (unsigned int i = 0; i < layers_quantity; i++)
		threads[i].reset(new sf::Thread(std::bind(&Layers::update_selected, this, i)));
}

bool Layers::all_populations_dead()
{
	for (auto& el : populations)
		if (!el.all_agents_dead())
			return false;
	return true;
}

void Layers::set_best_population()
{
	best_population = static_cast<int>(std::max_element(populations.begin(), populations.end(), [](const Population& population_1, const Population& population_2)
	{ return population_1.fitness_sum < population_2.fitness_sum; }) - populations.begin());
}

void Layers::update()
{
	for (auto& el : threads)
		el->launch();
	for (auto& el : threads)
		el->wait();
}

void Layers::update_selected(const int& i)
{
	if (populations[i].all_agents_dead())
	{
		populations[i].calculate_fitness();
		populations[i].set_best_agent();
		populations[i].calculate_fitness_sum();
		set_best_population();
		populations[i].natural_selection();
		populations[i].mutate();
		if (populations[i].reached_the_goal) populations[i].after_reach++;
	}
	else
		populations[i].update();
}

void Layers::show(sf::RenderWindow& window)
{
	for (auto& el : populations)
		el.show(window);
}
#pragma once
#define PI 3.1415926535
#include <raylib.h>
#include <vector>
#include <algorithm>
#include "Cell.h"
#include "Ant.h"
#include "Utils.h"
#include "FoodSource.h"
#include "DirectionOption.h"

class Simulation
{
public:
	Simulation(int width,
		int height,
		Vector2 size_cell,
		int count_ants,
		float evaporation_rate,
		float pheromone_deposit,
		float speed_ant,
		int radius_home,
		int count_food_sources,
		int radius_food_source,
		int count_food_to_source,
		int supply_pheromone);
	void init();
	void update(float dt);

	std::vector<Ant> ants;
	std::vector<std::vector<Cell>> map;
	std::vector<FoodSource> food_sources;
	Vector2 size_map;
	Vector2 home_position;
	int count_food_sources;
	int radius_food_source;
	int count_food_to_source;
	float supply_pheromone;

	int width;
	int height;
	Vector2 size_cell;
	int count_ants;
	float evaporation_rate;
	float pheromone_deposit;
	float speed_ant;
	int radius_home;
};

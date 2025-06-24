#include "../include/Simulation.h"

using namespace utils;

Simulation::Simulation(int width, int height, Vector2 size_cell, int count_ants, float evaporation_rate, float pheromone_deposit, float speed_ant, int radius_home)
{
	this->width = width;
	this->height = height;
	this->size_cell = size_cell;
	this->count_ants = count_ants;
	this->evaporation_rate = evaporation_rate;
	this->pheromone_deposit = pheromone_deposit;
	this->speed_ant = speed_ant;
	this->radius_home = radius_home;
}
void Simulation::init()
{
	home_position = { randomFloat(width / 4, (width / 4) * 3), randomFloat(height / 4, (height / 4) * 3) };
	size_map = { width / size_cell.x, height / size_cell.y };
	for (int i = 0; i < count_ants; ++i)
	{
		ants.push_back(Ant(randomFloat(0.f, PI * 2), home_position, Target::FOOD));
	}
	for (int i = 0; i < size_map.y; ++i)
	{
		map.push_back(std::vector<Cell>());
		for (int j = 0; j < size_map.x; ++j)
		{
			map[i].push_back(Cell(j, i));
		}
	}
	for (int i = 0; i < 5; ++i)
	{
		food_sources.push_back(FoodSource({ randomFloat(width / 4, (width / 4) * 3), randomFloat(height / 4, (height / 4) * 3) }, 50, 10));
	}
}
void Simulation::update(float dt)
{
	for (auto& row : map)
	{
		for (auto& cell : row)
		{
			cell.pheromone_food *= evaporation_rate;
			cell.pheromone_home *= evaporation_rate;
		}
	}
	for (auto& ant : ants)
	{
		int x = (int)(ant.position.x / size_cell.x);
		int y = (int)(ant.position.y / size_cell.y);

		float target_angle = ant.angle;
		float max = -1.0f;
		for (float i = -PI / 8; i <= PI; i += PI / 8)
		{
			float angle = ant.angle + i;
			Vector2 direction = getDirection(angle);
			Vector2 pos = { ant.position.x + direction.x * size_cell.x * 3 , ant.position.y + direction.y * size_cell.y * 3 };
			Vector2 cell_pos = { (int)(pos.x / size_cell.x), (int)(pos.y / size_cell.y) };
			if (outOfBounds(size_map, cell_pos)) continue;
			if (ant.target == Target::FOOD)
			{
				if (map[cell_pos.y][cell_pos.x].pheromone_food > max)
				{
					max = map[cell_pos.y][cell_pos.x].pheromone_food;
					target_angle = angle;
				}
			}
			else
			{
				if (map[cell_pos.y][cell_pos.x].pheromone_home > max)
				{
					max = map[cell_pos.y][cell_pos.x].pheromone_home;
					target_angle = angle;
				}
			}
		}
		if (max <= 0.1f)
		{
			float random_offset = randomFloat(-PI / 4, PI / 4);
			target_angle = ant.angle + random_offset;
		}

		ant.angle = target_angle;
		ant.angle = fmod(ant.angle, 2 * PI);
		if (ant.angle < 0) ant.angle += 2 * PI;
		Vector2 direction = getDirection(target_angle);
		Vector2 next_pos =
		{
			ant.position.x + direction.x * dt * speed_ant,
			ant.position.y + direction.y * dt * speed_ant
		};
		int grid_x = (int)(next_pos.x / size_cell.x);
		int grid_y = (int)(next_pos.y / size_cell.y);
		Vector2 grid_pos = { grid_x, grid_y };
		if (!outOfBounds(size_map, grid_pos))
		{
			ant.position.x = next_pos.x;
			ant.position.y = next_pos.y;
			if (ant.target == Target::FOOD)
			{
				map[grid_y][grid_x].pheromone_home = std::clamp(map[grid_y][grid_x].pheromone_home + pheromone_deposit, 0.f, 1.f);
				Vector2 ant_pos = { ant.position.x, ant.position.y };

				auto it = std::find_if(food_sources.begin(), food_sources.end(),
					[&](const FoodSource& food_source) {
						return circle_collision(ant_pos, food_source.position, food_source.radius);
					});

				if (it != food_sources.end())
				{
					ant.target = Target::HOME;
					if (--(*it).count_food == 0)
					{
						food_sources.erase(it);
					}
					continue;
				}
			}
			else
			{
				map[grid_y][grid_x].pheromone_food = std::clamp(map[grid_y][grid_x].pheromone_food + pheromone_deposit, 0.f, 1.f);
				Vector2 ant_pos = { ant.position.x, ant.position.y };
				if (circle_collision(ant_pos, home_position, radius_home))
				{
					ant.target = Target::FOOD;
					continue;
				}
			}
		}
		else
		{
			ant.angle += PI;
		}
	}
}
#include "../include/Simulation.h"

using namespace utils;

Simulation::Simulation(int width, int height, Vector2 size_cell, int count_ants, float evaporation_rate, float pheromone_deposit, float speed_ant)
{
	this->width = width;
	this->height = height;
	this->size_cell = size_cell;
	this->count_ants = count_ants;
	this->evaporation_rate = evaporation_rate;
	this->pheromone_deposit = pheromone_deposit;
	this->speed_ant = speed_ant;
}
void Simulation::init()
{
	for (int i = 0; i < count_ants; ++i)
	{
		ants.push_back(Ant(randomFloat(0.f, PI * 2), home_position, Target::FOOD));
	}
	for (int i = 0; i < size_map.y; ++i)
	{
		map.push_back(std::vector<Cell>());
		for (int j = 0; j < size_map.x; ++j)
		{
			map[i].push_back(Cell());
		}
	}
}
void Simulation::update(float dt)
{
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
			Vector2 pos = { ant.x + direction.x * size_cell.x , ant.y + direction.y * size_cell.y };
			Vector2 cell_pos = { (int)(pos.x / size_cell.x), (int)(pos.y / size_cell.y) };
			if (outOfBounds(size_map, cell_pos)) continue;
			if (ant.target)
			{
				if (pheromone_to_food[cell_pos.y][cell_pos.x] > max)
				{
					max = pheromone_to_food[cell_pos.y][cell_pos.x];
					target_angle = angle;
				}
			}
			else
			{
				if (pheromone_to_home[cell_pos.y][cell_pos.x] > max)
				{
					max = pheromone_to_home[cell_pos.y][cell_pos.x];
					target_angle = angle;
				}
			}
		}
		if (max <= 0.5f)
		{
			float random_offset = randomFloat(-PI / 8, PI / 8);
			target_angle = ant.angle + random_offset;
		}

		ant.angle = target_angle;
		Vector2 direction = getDirection(target_angle);
		Vector2 next_pos =
		{
			ant.x + direction.x * dt * SPEED_ANT,
			ant.y + direction.y * dt * SPEED_ANT
		};
		int grid_x = (int)(next_pos.x / size_cell.x);
		int grid_y = (int)(next_pos.y / size_cell.y);
		Vector2 grid_pos = { grid_x, grid_y };
		if (!outOfBounds(size_map, grid_pos))
		{
			ant.x = next_pos.x;
			ant.y = next_pos.y;
			if (ant.target)
			{
				pheromone_to_home[y][x] = std::clamp(pheromone_to_home[y][x] + PHEROMONE_DEPOSIT, 0.f, 1.f);
				Vector2 ant_pos = { ant.x, ant.y };

				auto it = std::find_if(foods_positions.begin(), foods_positions.end(),
					[&](const Vector2& food_pos) {
						return circle_collision(ant_pos, food_pos, radius_food);
					});

				if (it != foods_positions.end())
				{
					ant.target = !ant.target;
					foods_positions.erase(it);
					continue;
				}
			}
			else
			{
				pheromone_to_food[y][x] = std::clamp(pheromone_to_food[y][x] + PHEROMONE_DEPOSIT, 0.f, 1.f);
				Vector2 ant_pos = { ant.x, ant.y };
				if (circle_collision(ant_pos, home_pos, radius_home))
				{
					ant.target = !ant.target;
					ant.angle += PI;
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
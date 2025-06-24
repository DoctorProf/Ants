#include "../include/Simulation.h"

using namespace utils;

Simulation::Simulation(int width,
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
	int supply_pheromone)
{
	this->width = width;
	this->height = height;
	this->size_cell = size_cell;
	this->count_ants = count_ants;
	this->evaporation_rate = evaporation_rate;
	this->pheromone_deposit = pheromone_deposit;
	this->speed_ant = speed_ant;
	this->radius_home = radius_home;
	this->count_food_sources = count_food_sources;
	this->radius_food_source = radius_food_source;
	this->count_food_to_source = count_food_to_source;
	this->supply_pheromone = supply_pheromone;
}
void Simulation::init()
{
	home_position = { randomFloat(width / 4, (width / 4) * 3), randomFloat(height / 4, (height / 4) * 3) };
	size_map = { width / size_cell.x, height / size_cell.y };
	for (int i = 0; i < count_ants; ++i)
	{
		ants.push_back(Ant(randomFloat(0.f, PI * 2), home_position, Target::FOOD, supply_pheromone));
	}
	for (int i = 0; i < size_map.y; ++i)
	{
		map.push_back(std::vector<Cell>());
		for (int j = 0; j < size_map.x; ++j)
		{
			map[i].push_back(Cell(j, i));
		}
	}
	for (int i = 0; i < count_food_sources; ++i)
	{
		food_sources.push_back(FoodSource({ randomFloat(width / 4, (width / 4) * 3), randomFloat(height / 4, (height / 4) * 3) }, count_food_to_source, radius_food_source));
	}
}
//void Simulation::update(float dt)
//{
//	for (auto& row : map)
//	{
//		for (auto& cell : row)
//		{
//			cell.pheromone_food *= evaporation_rate;
//			cell.pheromone_home *= evaporation_rate;
//		}
//	}
//
//	for (auto& ant : ants)
//	{
//		int x = (int)(ant.position.x / size_cell.x);
//		int y = (int)(ant.position.y / size_cell.y);
//
//		float target_angle = ant.angle;
//		std::vector<DirectionOption> options;
//
//		for (float offset = -PI / 4; offset <= PI / 4; offset += PI / 8)
//		{
//			float angle = ant.angle + offset;
//			Vector2 dir = getDirection(angle);
//			Vector2 pos = { ant.position.x + dir.x * size_cell.x * 5, ant.position.y + dir.y * size_cell.y * 5 };
//			Vector2 grid_pos = { (int)(pos.x / size_cell.x), (int)(pos.y / size_cell.y) };
//
//			if (outOfBounds(size_map, grid_pos)) continue;
//
//			float pheromone = (ant.target == Target::FOOD)
//				? map[(int)grid_pos.y][(int)grid_pos.x].pheromone_food
//				: map[(int)grid_pos.y][(int)grid_pos.x].pheromone_home;
//
//			options.push_back({ angle, pheromone + 0.01f });
//		}
//
//		if (!options.empty())
//		{
//			float total_weight = 0.f;
//			for (auto& o : options) total_weight += o.weight;
//
//			float r = randomFloat(0.f, total_weight);
//			float accum = 0.f;
//			for (auto& o : options)
//			{
//				accum += o.weight;
//				if (r <= accum)
//				{
//					target_angle = o.angle;
//					break;
//				}
//			}
//		}
//
//		ant.angle = fmod(target_angle, 2 * PI);
//		if (ant.angle < 0) ant.angle += 2 * PI;
//
//		Vector2 dir = getDirection(ant.angle);
//		Vector2 next_pos =
//		{
//			ant.position.x + dir.x * dt * speed_ant,
//			ant.position.y + dir.y * dt * speed_ant
//		};
//
//		bool is_occupied = std::any_of(ants.begin(), ants.end(), [&](const Ant& other) {
//			if (&ant == &other) return false;
//			return distance(other.position, next_pos) < 2.0f;
//			});
//
//		if (is_occupied)
//		{
//			ant.angle += randomFloat(PI / 4, PI / 2);
//			continue;
//		}
//
//		int grid_x = (int)(next_pos.x / size_cell.x);
//		int grid_y = (int)(next_pos.y / size_cell.y);
//		Vector2 grid_pos = { grid_x, grid_y };
//
//		if (!outOfBounds(size_map, grid_pos))
//		{
//			ant.position = next_pos;
//			Vector2 ant_pos = ant.position;
//
//			if (ant.target == Target::FOOD)
//			{
//				float dist = distance(ant.position, home_position);
//				float factor = std::clamp(1.0f - dist / 1000.f, 0.f, 1.f);
//				float amount = std::min(pheromone_deposit * factor, ant.pheromone_supply);
//				map[grid_y][grid_x].pheromone_home = std::clamp(map[grid_y][grid_x].pheromone_home + amount, 0.f, 1.f);
//				//ant.pheromone_supply -= amount;
//
//				auto it = std::find_if(food_sources.begin(), food_sources.end(),
//					[&](const FoodSource& source) {
//						return circle_collision(ant_pos, source.position, source.radius / 1.2);
//					});
//
//				if (it != food_sources.end())
//				{
//					ant.target = Target::HOME;
//					ant.angle += PI;
//					ant.pheromone_supply = supply_pheromone;
//					if (--it->count_food <= 0)
//					{
//						food_sources.erase(it);
//					}
//					continue;
//				}
//			}
//			else
//			{
//				float amount = std::min(pheromone_deposit, ant.pheromone_supply);
//				map[grid_y][grid_x].pheromone_food = std::clamp(map[grid_y][grid_x].pheromone_food + amount, 0.f, 1.f);
//				//ant.pheromone_supply -= amount;
//
//				if (circle_collision(ant_pos, home_position, radius_home / 1.2))
//				{
//					ant.target = Target::FOOD;
//					ant.angle += PI;
//					ant.pheromone_supply = supply_pheromone;
//					continue;
//				}
//			}
//		}
//		else
//		{
//			ant.angle += PI;
//		}
//	}
//}
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
		float target_angle = ant.angle;
		float max_pheromone = 0.0f;
		std::vector<DirectionOption> options;

		for (float offset = -PI / 4; offset <= PI / 4; offset += PI / 16)
		{
			float angle = ant.angle + offset;
			Vector2 dir = getDirection(angle);
			Vector2 pos =
			{
				ant.position.x + dir.x * size_cell.x * 5,
				ant.position.y + dir.y * size_cell.y * 5
			};
			Vector2 grid_pos =
			{
				floor(pos.x / size_cell.x),
				floor(pos.y / size_cell.y)
			};

			if (utils::outOfBounds(size_map, grid_pos)) continue;

			float pheromone = (ant.target == Target::FOOD)
				? map[(int)grid_pos.y][(int)grid_pos.x].pheromone_food
				: map[(int)grid_pos.y][(int)grid_pos.x].pheromone_home;

			pheromone = powf(pheromone, 2.0f);

			DirectionOption option;
			option.angle = angle;
			option.weight = pheromone;
			options.push_back(option);

			if (pheromone > max_pheromone)
			{
				max_pheromone = pheromone;
				target_angle = angle;
			}
		}

		if (max_pheromone > 0.1f && randomFloat(0.f, 1.f) > 0.05f)
		{
			ant.angle = target_angle;
		}
		else
		{
			ant.angle += randomFloat(-PI / 4, PI / 4);
		}

		ant.angle = fmod(ant.angle, 2 * PI);
		if (ant.angle < 0) ant.angle += 2 * PI;

		Vector2 dir = getDirection(ant.angle);
		Vector2 next_pos =
		{
			ant.position.x + dir.x * dt * speed_ant,
			ant.position.y + dir.y * dt * speed_ant
		};

		/*bool is_occupied = std::any_of(ants.begin(), ants.end(), [&](const Ant& other)
			{
				if (&ant == &other) return false;
				return distance(other.position, next_pos) < 2.0f;
			});

		if (is_occupied)
		{
			ant.angle += randomFloat(-PI / 4, PI / 4);
			continue;
		}*/

		Vector2 grid_pos =
		{
			floor(next_pos.x / size_cell.x),
			floor(next_pos.y / size_cell.y)
		};

		if (!utils::outOfBounds(size_map, grid_pos))
		{
			ant.position = next_pos;
			int grid_x = (int)grid_pos.x;
			int grid_y = (int)grid_pos.y;
			Vector2 ant_pos = ant.position;

			if (ant.target == Target::FOOD)
			{
				float dist = distance(ant.position, home_position);
				float factor = std::clamp(1.0f - dist / 1000.f, 0.1f, 1.f);
				float amount = pheromone_deposit * factor * 2.0f;

				map[grid_y][grid_x].pheromone_home = std::clamp(
					map[grid_y][grid_x].pheromone_home + amount,
					0.f,
					1.f
				);

				auto it = std::find_if(food_sources.begin(), food_sources.end(),
					[&](const FoodSource& source) {
						return circle_collision(ant_pos, source.position, source.radius);
					});

				if (it != food_sources.end())
				{
					ant.target = Target::HOME;
					ant.angle += PI + randomFloat(-PI / 8, PI / 8);

					for (int i = -2; i <= 2; i++)
					{
						for (int j = -2; j <= 2; j++)
						{
							int y = grid_y + i;
							int x = grid_x + j;
							Vector2 check_pos = { (float)x, (float)y };
							if (!utils::outOfBounds(size_map, check_pos))
							{
								map[y][x].pheromone_home = std::clamp(
									map[y][x].pheromone_home + amount * 0.5f,
									0.f,
									1.f
								);
							}
						}
					}

					if (--it->count_food <= 0) {
						food_sources.erase(it);
					}
				}
			}
			else
			{
				float dist = distance(ant.position, home_position);
				float factor = std::clamp(dist / 500.f, 0.1f, 1.f);
				float amount = pheromone_deposit * factor * 2.0f;

				map[grid_y][grid_x].pheromone_food = std::clamp(
					map[grid_y][grid_x].pheromone_food + amount,
					0.f,
					1.f
				);

				if (circle_collision(ant_pos, home_position, radius_home))
				{
					ant.target = Target::FOOD;
					ant.angle += PI + randomFloat(-PI / 8, PI / 8);

					for (int i = -2; i <= 2; i++)
					{
						for (int j = -2; j <= 2; j++)
						{
							int y = grid_y + i;
							int x = grid_x + j;
							Vector2 check_pos = { (float)x, (float)y };
							if (!utils::outOfBounds(size_map, check_pos))
							{
								map[y][x].pheromone_food = std::clamp(
									map[y][x].pheromone_food + amount * 0.5f,
									0.f,
									1.f
								);
							}
						}
					}
				}
			}
		}
		else
		{
			ant.angle += randomFloat(-PI / 2, PI / 2);
		}
	}
}
#include "include/Simulation.h"
int main()
{
	InitWindow(1280, 720, "Ants");
	SetTargetFPS(60);

	const int WIDTH = 1280;
	const int HEIGHT = 720;

	Simulation simulation = Simulation(WIDTH, HEIGHT, { 5, 5 }, 1000, 0.995f, 0.1f, 120.f, 30, 1, 20, 1000, 500.f);

	simulation.init();

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

		simulation.update(dt);

		BeginDrawing();
		ClearBackground(BLACK);
		for (auto& row : simulation.map)
		{
			for (auto& cell : row)
			{
				if (cell.pheromone_food < 0.05 && cell.pheromone_home < 0.05)
				{
					continue;
				}
				float value_color_food = cell.pheromone_food * 255;
				float value_color_home = cell.pheromone_home * 255;
				Color color =
				{
					(unsigned char)value_color_food,
					(unsigned char)value_color_home,
					0,
					127
				};
				DrawRectangle(cell.x * simulation.size_cell.x, cell.y * simulation.size_cell.y, simulation.size_cell.x, simulation.size_cell.y, color);
			}
		}
		for (auto& ant : simulation.ants) {
			Color color = ant.target == Target::FOOD ? BLUE : RED;
			DrawCircle(ant.position.x, ant.position.y, 2, color);
		}
		DrawCircle(simulation.home_position.x, simulation.home_position.y, simulation.radius_home, Color(0, 255, 0, 255));
		for (auto& source : simulation.food_sources)
		{
			DrawCircle(source.position.x, source.position.y, source.radius, Color(255, 0, 0, 255));
			DrawText(std::to_string(source.count_food).c_str(), source.position.x - source.radius / 2, source.position.y - source.radius / 2, 12, Color(255, 255, 255, 255));
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
#include "include/Simulation.h"
int main()
{
	InitWindow(1920, 1080, "Ants");
	//SetTargetFPS(60);

	const int WIDTH = 1920;
	const int HEIGHT = 1080;

	Vector2 size_cell = { 5, 5 };
	Vector2 size_map = { WIDTH / size_cell.x, HEIGHT / size_cell.y };
	Vector2 home_pos = { randomFloat(WIDTH / 4, (WIDTH / 4) * 3), randomFloat(HEIGHT / 4, (HEIGHT / 4) * 3) };
	int count_ants = 1;
	const float EVAPORATION_RATE = 1.f;
	const float PHEROMONE_DEPOSIT = 0.9f;
	const float SPEED_ANT = 320.f;

	Simulation simulation = Simulation(1920, 1080, { 5, 5 }, 100, 0.99f, 0.1f, 120.f);

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

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
				cell.pheromone_food = std::max(0.0f, cell.pheromone_food * EVAPORATION_RATE);
				cell.pheromone_home = std::max(0.0f, cell.pheromone_home * EVAPORATION_RATE);
				float value_color_food = cell.pheromone_food * 255;
				float value_color_home = cell.pheromone_home * 255;
				Color color =
				{
					(unsigned char)value_color_food,
					(unsigned char)value_color_home,
					0,
					127
				};
				DrawRectangle(cell.x * size_cell.x, cell.y * size_cell.y, size_cell.x, size_cell.y, color);
			}
		}
		DrawCircle(home_pos.x, home_pos.y, radius_home, Color(0, 255, 0, 255));
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
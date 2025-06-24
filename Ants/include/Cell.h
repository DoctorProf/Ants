#pragma once

class Cell
{
public:
	Cell(int x, int y);
	float pheromone_food;
	float pheromone_home;
	int x;
	int y;
};

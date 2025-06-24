#include "../include/Cell.h"

Cell::Cell(int x, int y)
{
	this->x = x;
	this->y = y;
	pheromone_food = 0.f;
	pheromone_home = 0.f;
}
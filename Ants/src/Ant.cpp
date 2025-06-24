#include "../include/Ant.h"
Ant::Ant(float angle, Vector2 position, Target target, float pheromone_supply)
{
	this->position = position;
	this->angle = angle;
	this->target = target;
	this->pheromone_supply = pheromone_supply;
}
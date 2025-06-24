#pragma once
#include <raylib.h>

class FoodSource
{
public:
	FoodSource(Vector2 position, int count_food, int radius);
	Vector2 position;
	int count_food;
	int radius;
};

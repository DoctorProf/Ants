#pragma once
#include <iostream>
#include <boost/random.hpp>
#include <random>
#include <raylib.h>

namespace utils
{
	float getVecLength(Vector2 vec);
	Vector2 getDirection(Vector2 pos_1, Vector2 pos_2);
	float distance(Vector2 pos_1, Vector2 pos_2);
	Vector2 getDirection(float angle);
	float randomFloat(float min, float max);
	bool outOfBounds(Vector2& size_map, Vector2& pos);
	bool circle_collision(Vector2 unit_pos, Vector2 center_circle, float radius);
}

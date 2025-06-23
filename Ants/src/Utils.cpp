#include "../include/Utils.h"

float utils::getVecLength(Vector2 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}
Vector2 utils::getDirection(Vector2 pos_1, Vector2 pos_2)
{
	Vector2 dir = { pos_2.x - pos_1.x , pos_2.y - pos_1.y };
	float length = getVecLength(dir);
	dir = { dir.x / length, dir.y / length };
	return dir;
}
float utils::distance(Vector2 pos_1, Vector2 pos_2)
{
	float dx = pos_2.x - pos_1.x;
	float dy = pos_2.y - pos_1.y;
	return sqrtf(dx * dx + dy * dy);
}

Vector2 utils::getDirection(float angle)
{
	return Vector2(cos(angle), sin(angle));
}
float utils::randomFloat(float min, float max)
{
	static boost::random::mt19937 rng{ std::random_device{}() };
	boost::random::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}
bool utils::outOfBounds(Vector2& size_map, Vector2& pos)
{
	return pos.x < 0 || pos.y < 0 || pos.x >= size_map.x || pos.y >= size_map.y;
}
bool utils::circle_collision(Vector2 unit_pos, Vector2 center_circle, float radius)
{
	return distance(unit_pos, center_circle) <= radius;
}
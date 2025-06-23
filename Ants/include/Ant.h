#pragma once
#include <raylib.h>
#include "Target.h"

class Ant
{
public:
	Ant(float angle, Vector2 position, Target target);

	Vector2 position;
	float angle;
	Target target;
};

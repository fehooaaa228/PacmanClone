#pragma once

#include "vec2.h"

class Object
{
public:
	vec2 pos;

	Object() : pos(vec2(0, 0)) {}
	Object(float x, float y) : pos(vec2(x, y)) {}
};


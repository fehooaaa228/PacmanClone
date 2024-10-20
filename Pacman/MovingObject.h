#pragma once

#include <vector>
#include "Object.h"
#include "vec2.h"

class MovingObject : public Object
{
public:
	float speed;
	vec2 dir;
	float r;

	MovingObject();
	MovingObject(float x, float y, float _speed, float r);

	bool willCollide(vec2 p, char w);
	void correctPos();

private:
	int a = 0, b = 0;
};


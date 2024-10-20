#pragma once

#include "Object.h"
#include <raylib.h>

class Powerup : public Object {
public:
	Powerup(float x, float y);

	void draw(const Texture2D& texture);
};


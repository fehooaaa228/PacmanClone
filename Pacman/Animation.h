#pragma once

#include <raylib.h>
#include "Globals.h"

class Animation
{
public:
	Animation(Rectangle* frames = nullptr, int speed = 0, int framesCount = 0, bool loop = true);

	void nextFrame(const Texture2D& texture, const Rectangle& dest, float rotation = 0, bool inc = true);
	void restart();
	
private:
	int speed, frame = 0, framesCount;

	bool loop;

	Rectangle* frames;
};


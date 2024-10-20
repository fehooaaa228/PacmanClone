#include "Animation.h"

Animation::Animation(Rectangle* frames, int speed, int framesCount, bool loop) : 
	frames(frames), speed(speed), framesCount(framesCount), loop(loop) {}

void Animation::nextFrame(const Texture2D& texture, const Rectangle& dest, float rotation, bool inc)
{
	if (inc)
	{
		if ((int)(GetTime() * FPS) % speed == 0)
		{
			if (!(frame == framesCount - 1 && !loop))
			{
				frame++;
			}
		}
	}

	DrawTexturePro(texture, frames[frame % framesCount], dest, Vector2{ 0, 0 }, rotation, WHITE);
}

void Animation::restart()
{
	frame = 0;
}
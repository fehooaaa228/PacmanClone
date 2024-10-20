#pragma once

#include "Ghost.h"
#include "vec2.h"
#include <vector>
#include <raylib.h>

class GhostManager
{
public:
	Modes mode = SCATTERING;
	float frightenedModeDuration;
	int frightenedMode = frightenedModeDuration;
	int scatterModeDuration, chasingModeDuration;
	int wave = 1;

	GhostManager(int level = 1);

	void update(vec2 playerPos, vec2 playerDir, int dotsRemaining);
	void switchMode(Modes m, vec2 playerPos, vec2 playerDir, int dotsRemaining);
	void setTargets(Modes mode, vec2 playerPos, vec2 playerDir, int dotsRemaining);
	void setDurations();
	void drawGhosts(const Texture2D& animation);

	Ghost* getGhosts();

private:
	Ghost* ghosts = new Ghost[4];
	int switchCd = FPS * 7;
	Modes lastMode = mode;
	int level;
	int cruiseElroyMode = 0, keypoint1, keypoint2;
};


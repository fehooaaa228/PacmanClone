#pragma once

#include "MovingObject.h"
#include "Dot.h"
#include "Ghost.h"
#include <vector>
#include "GhostManager.h"
#include "Globals.h"
#include "Powerup.h"
#include <raylib.h>
#include "Animation.h"

class Player : public MovingObject
{
public:
	int lives = 4;
	bool alive = true;
	int killGain = 200;

	Player(float x = 0, float y = 0);

	void update(std::vector<Dot>* dots, std::vector<Powerup>* powerups, int* score, GhostManager* ghostManager, int* timer);
	void draw(const Texture2D& animation);
	void respawn(int level);
	void move();

private:
	vec2 targetDir;
	int r = C_SIZE;
	bool isMoving = false;
	Animation moveAnim, dieAnim;

	bool collides(vec2 p);
};
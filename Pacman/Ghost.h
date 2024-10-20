#pragma once

#include "MovingObject.h"
#include "Globals.h"
#include <raylib.h>
#include "vec2.h"
#include "Animation.h"

class Ghost : public MovingObject {
public:
	int delay;
	bool free = false;
	bool alive = true;
	bool frightened = false;
	vec2 target;

	Ghost(float x = 0, float y = 0, float speed = 0, int delay = 0, int id = 0);

	void update();
	void draw(const Texture2D& animation, int frightenedRemaining);
	void die();

	std::vector<vec2> findAvailableDirs();

private:
	int faze = 0;
	int frame = 0;
	int id;
	float r = C_SIZE;
	Animation defaultAnim, frightenedAnim, blinkAnim;
	bool isMoving = true;

	bool collides(vec2 p);

	void moveUpNDown();
	void moveToTarget();
	void moveOut();
	void moveRandomly();
	void moveToGhostHouse();
	void move(float speed);
};


#include "Ghost.h"
#include <raylib.h>

Ghost::Ghost(float x, float y, float speed, int delay, int id) : MovingObject(x, y, speed, C_SIZE), delay(delay), id(id)
{
	defaultAnim = Animation(new Rectangle[2]{
		Rectangle{0, 41.0f + 45.0f * id, 42, 42},
		Rectangle{42, 41.0f + 45.0f * id, 42, 42}
		}, GHOST_ANIMATION_SPEED, 2);

	frightenedAnim = Animation(new Rectangle[2]{
		Rectangle{84, 41, 42, 42},
		Rectangle{126, 41, 42, 42}
		}, GHOST_ANIMATION_SPEED, 2);

	blinkAnim = Animation(new Rectangle[2]{
		Rectangle{84, 86, 42, 42},
		Rectangle{126, 86, 42, 42}
		}, GHOST_ANIMATION_SPEED, 2);
}

bool Ghost::collides(vec2 p) {
	return willCollide(p, '@') || willCollide(p, 'x');
}

void Ghost::draw(const Texture2D& animation, int frightenedRemaining) {
	if ((int)(GetTime() * FPS) % GHOST_ANIMATION_SPEED == 0) frame++;

	Rectangle bodySource, eyesSource{ 0, 0, 0, 0 };
	Rectangle dest{ pos.x - SPRITE_SIZE / 2, pos.y - SPRITE_SIZE / 2, SPRITE_SIZE, SPRITE_SIZE };

	if (dir == dirs[0]) {
		eyesSource = Rectangle{ 60, 222, 30, 15 };
	}
	else if (dir == dirs[1]) {
		eyesSource = Rectangle{ 30, 222, 30, 15 };
	}
	else if (dir == dirs[2]) {
		eyesSource = Rectangle{ 90, 222, 30, 15 };
	}
	else if (dir == dirs[3]) {
		eyesSource = Rectangle{ 0, 222, 30, 15 };
	}
	if (alive) {
		if (frightened) {
			if (frightenedRemaining < FPS * 3)
			{
				if (frightenedRemaining % FPS >= 0 && frightenedRemaining % FPS <= FPS / 2)
				{
					blinkAnim.nextFrame(animation, dest);
				}
				else
				{
					frightenedAnim.nextFrame(animation, dest);
				}
			}
			else
			{
				frightenedAnim.nextFrame(animation, dest);
			}
		}
		else {
			defaultAnim.nextFrame(animation, dest);
			DrawTexturePro(animation, eyesSource,
				Rectangle{ (float)(pos.x - SPRITE_SIZE * 0.4 + dir.x * 4),
						(float)(pos.y - SPRITE_SIZE * 0.3 + dir.y * 4),
						SPRITE_SIZE * 0.8,
						SPRITE_SIZE * 0.4 },
				Vector2{ 0, 0 }, 0, WHITE);
		}
	}
	else
	{
		if ((int)(GetTime() * FPS) % 20 >= 10 && (int)(GetTime() * FPS) % 20 <= 20 && isMoving)
		{
			DrawTexturePro(animation, eyesSource,
				Rectangle{ (float)(pos.x - SPRITE_SIZE * 0.4 + dir.x * 4),
						(float)(pos.y - SPRITE_SIZE * 0.3 + dir.y * 4),
						SPRITE_SIZE * 0.8,
						SPRITE_SIZE * 0.4 },
				Vector2{ 0, 0 }, 0, WHITE);
		}
	}
}

void Ghost::update() {
	if (delay < 1) {
		isMoving = true;

		if (alive) {
			if (!free) {
				moveOut();
			}
			else {
				if (!frightened) moveToTarget();
				else moveRandomly();
			}
		}
		else {
			moveToGhostHouse();
		}
	}
	else {
		moveUpNDown();
		delay--;
	}
}

void Ghost::moveUpNDown() {
	if (pos.y < C_SIZE * 16 + 10) {
		dir = dirs[2];
	}
	else if (pos.y > C_SIZE * 17 + 10) {
		dir = dirs[0];
	}

	pos += dir * 0.5;
}

std::vector<vec2> Ghost::findAvailableDirs() {
	correctPos();

	bool restricted = pos == vec2(C_SIZE * 13, C_SIZE * 14) || pos == vec2(C_SIZE * 16, C_SIZE * 14) || pos == vec2(C_SIZE * 13, C_SIZE * 26) || pos == vec2(C_SIZE * 16, C_SIZE * 26);

	std::vector<vec2> ad;

	for (int i = 0; i < 4; i++) {
		if (dirs[i] != -dir && !collides(pos + dirs[i])) {
			if(!(restricted && dirs[i] == dirs[0])) ad.push_back(dirs[i]);
		}
	}

	return ad;
}

void Ghost::moveRandomly() {
	std::vector<vec2> ad = findAvailableDirs();
	if (ad.size() != 0)
	{
		dir = ad[rand() % ad.size()];
	}

	move(speed * 0.5);
}

void Ghost::moveOut() {
	if ((int)pos.x != S_WIDTH / 2) {
		dir = vec2(sign(S_WIDTH / 2 - pos.x), 0);
		pos += dir * 0.5;
	}
	else {
		dir = dirs[0];
		pos += dir * 0.5;

		if (ceil(pos.y) == SPACE_FOR_TEXT / 2 + C_SIZE * 12) {
			free = true;
			dir = dirs[1];
		}
	}
}

void Ghost::moveToTarget() {
	std::vector<vec2> ad = findAvailableDirs();

	vec2 nd;
	float mind = 10000;

	for (int i = 0; i < ad.size(); i++) {
		float l = length(target - (pos + ad[i]));
		if (l < mind) {
			mind = l;
			nd = ad[i];
		}
	}

	dir = nd;
	move(speed);
}

void Ghost::moveToGhostHouse() {
	switch (faze) {
	case 0:
		moveToTarget();

		if (length(target - pos) < 5) {
			pos.x = S_WIDTH / 2;
			faze++;
		}

		break;
	case 1:
		pos += dirs[2] * speed;

		if (pos.y > C_SIZE * 17) faze++;

		break;

	case 2:
		pos += dirs[0];

		if (length(target - pos) < 5) {
			pos = target;
			alive = true;
			speed /= 2;
			faze = 0;
		}

		break;
	}

}

void Ghost::move(float _speed) {
	pos += dir * _speed;
}

void Ghost::die() {
	target = vec2(S_WIDTH / 2, C_SIZE * 14);
	alive = false;
	if (!free) {
		free = true;
		faze = 1;
	}
	speed *= 2;
	frightened = false;
	isMoving = false;
}

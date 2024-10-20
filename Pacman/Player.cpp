#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player(float x, float y) : MovingObject(x, y, PLAYER_SPEED, C_SIZE), targetDir(dir) {
	moveAnim = Animation(new Rectangle[4]{
		Rectangle{0, 0, 41, 41},
		Rectangle{41, 0, 41, 41},
		Rectangle{82, 0, 41, 41},
		Rectangle{41, 0, 41, 41}
		}, PLAYER_ANIMATION_SPEED, 4);

	Rectangle* frames = new Rectangle[16];
	for (int i = -3; i < 13; i++)
	{
		frames[i + 3] = Rectangle{ 168.0f + 45 * ((i >= 0 ) ? i : 0), 0, 45, 41};
	}
	dieAnim = Animation(frames, PLAYER_ANIMATION_SPEED * 3, 16, false);
}

bool Player::collides(vec2 p) {
	return willCollide(p, '@');
}

void Player::update(std::vector<Dot>* dots, std::vector<Powerup>* powerups, int* score, GhostManager* ghostManager, int* timer) {
	if (IsKeyPressed(KEY_UP)) targetDir = dirs[0];
	if (IsKeyPressed(KEY_RIGHT)) targetDir = dirs[3];
	if (IsKeyPressed(KEY_DOWN)) targetDir = dirs[2];
	if (IsKeyPressed(KEY_LEFT)) targetDir = dirs[1];

	correctPos();

	if (dir != targetDir) {
		if (!collides(pos + targetDir)) dir = targetDir;
	}

	if (!collides(pos + dir)) {
		move();
		isMoving = true;
	}
	else {
		isMoving = false;
	}

	for (int i = 0; i < dots->size(); i++) {
		vec2 diff = (*dots)[i].pos - pos;
		if (diff.x * diff.x + diff.y * diff.y <= r * r * 0.8) {
			(*dots).erase((*dots).begin() + i);
			*score += 10;
			if (dots->size() == 0)
			{
				*timer = DELAYS;
				isMoving = false;
			}
			break;
		}
	}

	Ghost* ghosts = ghostManager->getGhosts();

	for (int i = 0; i < 4; i++) {
		vec2 nPos = ghosts[i].pos - pos;

		if (nPos.x * nPos.x + nPos.y * nPos.y <= r * r) {
			if (ghosts[i].frightened && ghosts[i].alive) {
				ghosts[i].die();
				*score += killGain;
				killGain *= 2;
				*timer = FPS * 1.5;
				isMoving = false;
			}
			else if (!ghosts[i].frightened && ghosts[i].alive) {
				lives--;
				alive = false;
				*timer = lives == 0 ? DELAYS * 1.5 : DELAYS;
			}
		}
	}

	for (int i = 0; i < powerups->size(); i++) {
		vec2 diff = (*powerups)[i].pos - pos;
		if (diff.x * diff.x + diff.y * diff.y <= r * r * 0.8) {
			*score += 50;
			(*powerups).erase((*powerups).begin() + i);
			ghostManager->switchMode(FRIGHTENED, pos, dir, dots->size());
			killGain = 200;

			break;
		}
	}

	if (pos.x <= 0 || pos.x > S_WIDTH) pos.x = abs(pos.x - S_WIDTH);
	if (pos.y <= 0 || pos.y > S_HEIGHT) pos.y = abs(pos.y - S_HEIGHT);
}

void Player::move()
{
	pos += dir * speed;
}

void Player::draw(const Texture2D& animation) {
	float rotation = 0;
	Rectangle dest;

	if (dir == dirs[0])
	{
		rotation = 270;
		dest = Rectangle{ pos.x - SPRITE_SIZE / 2, pos.y + SPRITE_SIZE / 2, SPRITE_SIZE, SPRITE_SIZE };
	}
	else if (dir == dirs[1])
	{
		rotation = 180;
		dest = Rectangle{ pos.x + SPRITE_SIZE / 2, pos.y + SPRITE_SIZE / 2, SPRITE_SIZE, SPRITE_SIZE };
	}
	else if (dir == dirs[2])
	{
		rotation = 90;
		dest = Rectangle{ pos.x + SPRITE_SIZE / 2, pos.y - SPRITE_SIZE / 2, SPRITE_SIZE, SPRITE_SIZE };
	}
	else if (dir == dirs[3])
	{
		rotation = 0;
		dest = Rectangle{ pos.x - SPRITE_SIZE / 2, pos.y - SPRITE_SIZE / 2, SPRITE_SIZE, SPRITE_SIZE };
	}

	if (alive)
	{
		moveAnim.nextFrame(animation, dest, rotation, isMoving);
	}
	else
	{
		dieAnim.nextFrame(animation, Rectangle{pos.x - SPRITE_SIZE / 2, pos.y - SPRITE_SIZE / 2, SPRITE_SIZE, SPRITE_SIZE});
	}
}

void Player::respawn(int level)
{
	speed = (speed < 1.1) ? PLAYER_SPEED + 0.016 * (level - 1) : 1.1;
	pos = vec2(S_WIDTH / 2, C_SIZE * 26);
	alive = true;
	dieAnim.restart();
	dir = dirs[1];
	targetDir = dirs[1];
	isMoving = false;
}
#include "MovingObject.h"
#include "Globals.h"
#include <cmath>
#include <iostream>

MovingObject::MovingObject() {}
MovingObject::MovingObject(float x, float y, float _speed, float r) : Object(x, y), speed(_speed), r(r), dir(vec2(-1, 0)) {}

bool MovingObject::willCollide(vec2 p, char w) {
	float nx = p.x / (float)C_SIZE;
	float ny = p.y / (float)C_SIZE;

	return map[ceil(ny)][ceil(abs(nx))] == w ||
		map[ceil(ny)][floor(abs(nx))] == w ||
		map[floor(ny)][ceil(abs(nx))] == w ||
		map[ceil(ny)][floor(abs(nx - 1))] == w ||
		map[floor(ny - 1)][ceil(abs(nx))] == w ||
		map[floor(ny - 1)][floor(abs(nx))] == w ||
		map[floor(ny)][floor(abs(nx - 1))] == w ||
		map[floor(ny)][floor(abs(nx))] == w ||
		map[floor(ny - 1)][floor(abs(nx - 1))] == w;
}

void MovingObject::correctPos() {
	if (a % 4 == 0) {
		if ((int)pos.x % C_SIZE != 0) {
			if ((int)pos.x % C_SIZE <= (int)speed + 1 && dir == dirs[1]) {
				pos.x = floor(pos.x / C_SIZE) * C_SIZE;
				a++;
			}
			else if ((int)pos.x % C_SIZE >= C_SIZE - (int)speed - 1 && dir == dirs[3]) {
				pos.x = ceil(pos.x / C_SIZE) * C_SIZE;
				a++;
			}
		}
	}
	else a++;

	if (b % 4 == 0) {
		if ((int)pos.y % C_SIZE != 0) {
			if ((int)pos.y % C_SIZE <= (int)speed + 1 && dir == dirs[0]) {
				pos.y = floor(pos.y / C_SIZE) * C_SIZE;
				b++;
			}
			else if ((int)pos.y % C_SIZE >= C_SIZE - (int)speed - 1 && dir == dirs[2]) {
				pos.y = ceil(pos.y / C_SIZE) * C_SIZE;
				b++;
			}
		}
	}
	else b++;
}
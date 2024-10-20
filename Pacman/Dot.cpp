#include "Dot.h"
#include <raylib.h>
#include "Globals.h"

Dot::Dot(float x, float y) : Object(x, y) {}

void Dot::draw() {
	float a = C_SIZE / 4;
	DrawRectangle(pos.x - a / 2, pos.y - a / 2, a, a, Color{255, 184, 174, 255});
}
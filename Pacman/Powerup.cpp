#include "Powerup.h"
#include "Globals.h"

Powerup::Powerup(float x, float y) : Object(x, y) {}

void Powerup::draw(const Texture2D& texture) {
	if ((int)(GetTime() * FPS) % 100 >= 0 && (int)(GetTime() * FPS) % 100 <= 50)
	{
		DrawTexturePro(texture, Rectangle{ 124, 222, 24, 24 }, Rectangle{ pos.x - 12, pos.y - 12, 24, 24 }, Vector2{ 0, 0 }, 0, WHITE);
	}
}
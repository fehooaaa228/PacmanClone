#include "Application.h"
#include <iostream>

Application::Application() {
	InitWindow(S_WIDTH, S_HEIGHT, "Pacman");
	SetTargetFPS(FPS);

	Image img = LoadImage("imgs/map.png");
	ImageResize(&img, S_WIDTH, S_HEIGHT - SPACE_FOR_TEXT);
	background = LoadTextureFromImage(img);
	UnloadImage(img);

	img = LoadImage("imgs/lightMap.png");
	ImageResize(&img, S_WIDTH, S_HEIGHT - SPACE_FOR_TEXT);
	lightBg = LoadTextureFromImage(img);
	UnloadImage(img);

	img = LoadImage("imgs/textures.png");
	animation = LoadTextureFromImage(img);
	UnloadImage(img);

	img = LoadImage("imgs/font.png");
	text = LoadTextureFromImage(img);
	UnloadImage(img);

	img = LoadImage("imgs/menu.png");
	menu = LoadTextureFromImage(img);
	UnloadImage(img);

	img = LoadImage("imgs/fruits.png");
	fruits = LoadTextureFromImage(img);
	UnloadImage(img);

	Rectangle* frames1 = new Rectangle[18];
	for (int i = 0; i < 18; i++)
	{
		frames1[i] = Rectangle{ 60.0f + 665 * i, 156, 600, 259};
	}
	menuAnim = Animation(frames1, MENU_ANIMATION_SPEED, 18);
}

void Application::run() 
{
	initMembers();

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		if (!inMenu)
		{
			if (timer == 0)
			{
				drawGameStaff();
				drawUI();
				drawConsumables();

				ghostManager.update(p.pos, p.dir, dots.size());
				ghostManager.drawGhosts(animation);

				p.update(&dots, &powerups, &score, &ghostManager, &timer);
				if ((242 - dots.size() == 70 || (242 - dots.size() == 170 && fruit.timer == 0)) && fruit.timer == 0)
				{
					if (level == 1 || level == 2)
					{
						fruit = Fruit(level - 1);
					}
					else if (level > 2 && level < 15)
					{
						fruit = Fruit((int)ceil((float)level / 2) % 8);
					}
					else
					{
						fruit = Fruit(7);
					}
				}

				if (fruit.timer != 0 && !fruit.collected)
				{
					vec2 dif = fruit.pos - p.pos;

					if (dif.x * dif.x + dif.y * dif.y <= C_SIZE * C_SIZE)
					{
						fruit.collected = true;
						fruit.timer = FPS * 2;
						score += fruitGain[fruit.id];
					}
				}

				if (score > highScore) highScore = score;

				p.draw(animation);
			}
			else
			{
				timer--;

				drawGameStaff();
				drawUI();

				if (p.alive)
				{
					if (dots.size() == 0)
					{
						drawConsumables();
						if (timer > DELAYS * 0.8)
						{
							ghostManager.drawGhosts(animation);
						}
						else
						{
							if ((int)(GetTime() * FPS) % FPS >= 0 && (int)(GetTime() * FPS) % FPS <= FPS / 2)
							{
								DrawTexture(lightBg, 0, SPACE_FOR_TEXT / 2, WHITE);
							}
						}
						p.draw(animation);

						if (timer == 0)
						{
							restart();
						}
					}
					else if (ghostManager.mode == FRIGHTENED)
					{
						drawConsumables();
						ghostManager.drawGhosts(animation);
						DrawTexturePro(text, Rectangle{ 0, (float)p.killGain / 8, 16, 7 }, Rectangle{ p.pos.x - 24, p.pos.y - 10, 48, 21 }, Vector2{ 0, 0 }, 0, WHITE);
					}
					else
					{
						drawConsumables();
						ghostManager.drawGhosts(animation);
						p.draw(animation);

						DrawTexturePro(text, Rectangle{ 0, 0, 129, 23 }, Rectangle{ C_SIZE * 11.5, C_SIZE * 19.5, C_SIZE * 6, C_SIZE }, Vector2{ 0, 0 }, 0, WHITE);
					}
				}
				else
				{
					drawConsumables();

					if (timer > DELAYS * 0.8)
					{
						ghostManager.drawGhosts(animation);
					}

					p.draw(animation);

					if (timer < DELAYS * 0.8 && p.lives == 0)
					{
						DrawTexturePro(text, Rectangle{ 0, 23, 193, 23 }, Rectangle{ C_SIZE * 10, C_SIZE * 19.5, C_SIZE * 9, C_SIZE }, Vector2{ 0, 0 }, 0, WHITE);
					}

					if (timer == 0)
					{
						restart();
					}
				}
			}
		}
		else
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				inMenu = false;
				timer = FPS * 3;
			}

			drawUI();

			DrawTexturePro(menu, Rectangle{ 132, 120, 477, 21 }, Rectangle{ (float)S_WIDTH / 2 - 238, C_SIZE * 4, 477, 21 }, Vector2{ 0, 0 }, 0, WHITE);
			DrawTexturePro(menu, Rectangle{ 291, 461, 120, 69 }, Rectangle{ (float)S_WIDTH / 2 - 60, C_SIZE * 27, 120, 69 }, Vector2{ 0, 0 }, 0, WHITE);
			DrawTexturePro(menu, Rectangle{ 0, 56, 133, 21 }, Rectangle{ C_SIZE * 2, (float)S_HEIGHT - C_SIZE, 133, 21 }, Vector2{ 0, 0 }, 0, WHITE);

			DrawRectangle(S_WIDTH / 2 - 60 - C_SIZE * 1.5, C_SIZE * 27.375, C_SIZE * 0.25, C_SIZE * 0.25, Color{ 255, 184, 174, 255 });
			menuPowerup.draw(animation);

			menuAnim.nextFrame(menu, Rectangle{C_SIZE * 4, C_SIZE * 6, 600, 259});
		}

		EndDrawing();
	}
}

void Application::resetGhostsNPlayer() {
	p.respawn(level);

	ghostManager = GhostManager(level);
}

void Application::initMembers() {
	dots.clear();
	powerups.clear();
	fruit = Fruit();

	resetGhostsNPlayer();

	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			if (map[i][j] == '.') dots.push_back(Dot(j * C_SIZE + C_SIZE, i * C_SIZE + C_SIZE));
			if (map[i][j] == 'b') powerups.push_back(Powerup(j * C_SIZE + C_SIZE, i * C_SIZE + C_SIZE));
		}
	}
}

void Application::drawConsumables() {
	for (int i = 0; i < powerups.size(); i++) {
		powerups[i].draw(animation);
	}

	for (int i = 0; i < dots.size(); i++) {
		dots[i].draw();
	}

	if (fruit.timer != 0)
	{
		fruit.timer--;
		if (fruit.collected)
		{
			DrawTexturePro(text, Rectangle{ 24.0f + 22 * fruit.id, 51, 21, 7 }, Rectangle{ fruit.pos.x - 18, fruit.pos.y - 18, 63, 21 }, Vector2{ 0, 0 }, 0, WHITE);
		}
		else
		{
			DrawTexturePro(animation, Rectangle{ 192.0f + 48 * fruit.id, 206, 36, 42 }, Rectangle{ fruit.pos.x - 18, fruit.pos.y - 21, 36, 42 }, Vector2{ 0, 0 }, 0, WHITE);
		}
	}
}

void Application::restart() {
	if (p.lives == 0)
	{
		score = 0;
		level = 1;
		p.lives = 4;
		initMembers();
		menuAnim.restart();
		inMenu = true;
	}
	else if(dots.size() == 0)
	{
		level++;
		initMembers();
	}
	else
	{
		resetGhostsNPlayer();
	}

	timer = FPS * 3;
}

void Application::drawUI() {
	if ((int)(GetTime() * FPS) % FPS >= 0 && (int)(GetTime() * FPS) % FPS <= FPS * 0.6)
	{
		DrawTexturePro(menu, Rectangle{ 78, 0, 66, 21 }, Rectangle{ C_SIZE, 0, 66, 21 }, Vector2{ 0, 0 }, 0, WHITE);
	}
	DrawTexturePro(menu, Rectangle{ 219, 0, 237, 21 }, Rectangle{ (float)S_WIDTH / 2 - 118, 0, 237, 21 }, Vector2{ 0, 0 }, 0, WHITE);
	DrawText(TextFormat("%02i", score), C_SIZE * 2, C_SIZE, 32, Color{222, 222, 255, 255});
	DrawText(TextFormat("%02i", highScore), S_WIDTH / 2 - C_SIZE, C_SIZE, 32, Color{ 222, 222, 255, 255 });
}

void Application::drawGameStaff()
{
	DrawTexture(background, 0, SPACE_FOR_TEXT / 2, WHITE);

	DrawTexturePro(fruits, Rectangle{ 0, 48.0f * (level - 1), 324, 40 }, Rectangle{ (float)S_WIDTH - 336, C_SIZE * 34, 324, 40 }, Vector2{ 0, 0 }, 0, WHITE);

	for (int i = 0; i < p.lives; i++)
	{
		DrawTexturePro(animation, Rectangle{ 157, 214, 30, 33 }, Rectangle{ 50.0f * i + 50, C_SIZE * 35 + C_SIZE / 2, 30, 33 }, Vector2{ 0, 0 }, 180, WHITE);
	}
}
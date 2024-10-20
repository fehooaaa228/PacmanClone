#pragma once

#include "Globals.h"
#include "Dot.h"
#include "Powerup.h"
#include "Ghost.h"
#include "GhostManager.h"
#include "Player.h"
#include <vector>
#include <raylib.h>
#include <cmath>

struct Fruit
{
	vec2 pos = vec2(S_WIDTH / 2, C_SIZE * 20);
	int timer = 0, id = 0;
	bool collected = false;

	Fruit(int _id = -1) : id(_id), timer(((_id == -1) ? 0 : FPS * 10)) {}
};

class Application
{
public:
	Application();
	void run();
	void initMembers();
	void restart();
	void drawUI();
	void drawConsumables();
	void resetGhostsNPlayer();
	void drawGameStaff();

private:
	Player p;
	std::vector<Dot> dots;
	std::vector<Powerup> powerups;
	GhostManager ghostManager;
	Texture2D background, lightBg, animation, text, menu, fruits;
	Animation menuAnim;
	Powerup menuPowerup = Powerup(S_WIDTH / 2 - 70 - C_SIZE, C_SIZE * 29 + 6);
	Fruit fruit;

	unsigned int level = 11;
	int timer = DELAYS;
	int score = 0, highScore = 0;
	int fruitGain[8] = { 100, 300, 500, 700, 1000, 2000, 3000, 5000 };

	bool inMenu = true;
};


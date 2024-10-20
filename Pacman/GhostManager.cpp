#include "GhostManager.h"
#include "Globals.h"
#include <iostream>
#include <raylib.h>

GhostManager::GhostManager(int level) : level(level) {
	float ghostSpeed = GHOST_SPEED + 0.016 * (level - 1);
	if (ghostSpeed > 1.11) ghostSpeed = 1.11;

	ghosts[0] = Ghost(S_WIDTH / 2, SPACE_FOR_TEXT / 2 + C_SIZE * 12, ghostSpeed); //RED
	ghosts[1] = Ghost(ghosts[0].pos.x, ghosts[0].pos.y + C_SIZE * 3, ghostSpeed, FPS * (1 - level), 1); //PINK
	ghosts[2] = Ghost(ghosts[1].pos.x + C_SIZE * 2, ghosts[1].pos.y, ghostSpeed, FPS * (10 - level), 3); //ORANGE
	ghosts[3] = Ghost(ghosts[1].pos.x - C_SIZE * 2, ghosts[1].pos.y, ghostSpeed, FPS * (6 - level), 2); //BLUE

	ghosts[1].dir = dirs[2];
	ghosts[2].dir = dirs[0];
	ghosts[3].dir = dirs[0];

	ghosts[0].free = true;

	ghosts[0].target = vec2(C_SIZE * (map[0].size() - 4), 0);
	ghosts[1].target = vec2(C_SIZE * 4, 0);
	ghosts[2].target = vec2(0, C_SIZE * map.size());
	ghosts[3].target = vec2(C_SIZE * map[0].size(), C_SIZE * map.size());

	frightenedModeDuration = FPS * 9 - FPS * 9 / 19 * level;
	frightenedModeDuration = (frightenedModeDuration > FPS) ? frightenedModeDuration : FPS;

	keypoint1 = 160 + level * 5;
	keypoint2 = 80 + level * 5;

	setDurations();
}

Ghost* GhostManager::getGhosts() {
	return ghosts;
}

void GhostManager::setDurations() {
	if (level == 1) {
		switch (wave) {
		case 1:
			scatterModeDuration = FPS * 7;
			chasingModeDuration = FPS * 20;
			break;
		case 2:
			scatterModeDuration = FPS * 7;
			chasingModeDuration = FPS * 20;
			break;
		case 3:
			scatterModeDuration = FPS * 5;
			chasingModeDuration = FPS * 20;
			break;
		default:
			scatterModeDuration = FPS * 5;
			chasingModeDuration = 256000;
			break;
		}
	}
	else if (level > 1 && level < 5) {
		switch (wave) {
		case 1:
			scatterModeDuration = FPS * 7;
			chasingModeDuration = FPS * 20;
			break;
		case 2:
			scatterModeDuration = FPS * 7;
			chasingModeDuration = FPS * 20;
			break;
		case 3:
			scatterModeDuration = FPS * 5;
			chasingModeDuration = FPS * 1033;
			break;
		default:
			scatterModeDuration = FPS / 60;
			chasingModeDuration = 256000;
			break;
		}
	}
	else {
		switch (wave) {
		case 1:
			scatterModeDuration = FPS * 5;
			chasingModeDuration = FPS * 20;
			break;
		case 2:
			scatterModeDuration = FPS * 5;
			chasingModeDuration = FPS * 20;
			break;
		case 3:
			scatterModeDuration = FPS * 5;
			chasingModeDuration = FPS * 1037;
			break;
		default:
			scatterModeDuration = FPS / 60;
			chasingModeDuration = 256000;
			break;
		}
	}

	std::cout << "Wave: " << wave << "; level: " << level << "; scatDur: " << scatterModeDuration << "; chaseDur: " << chasingModeDuration << ";\n";
}

void GhostManager::update(vec2 playerPos, vec2 playerDir, int dotsRemaining) {
	if (mode != FRIGHTENED) {
		if (switchCd <= 0) {
			switchMode(mode == SCATTERING ? CHASE : SCATTERING, playerPos, playerDir, dotsRemaining);
		}
		else switchCd--;
	}
	else {
		if (frightenedMode <= 0) {
			switchMode(lastMode, playerPos, playerDir, dotsRemaining);
		}
		else {
			setTargets(lastMode, playerPos, playerDir, dotsRemaining);

			frightenedMode--;
		}
	}

	if (cruiseElroyMode == 0)
	{
		int ghostsFree = 0;

		for (int i = 0; i < 4; i++)
		{
			if (ghosts[i].free) ghostsFree++;
		}

		if (dotsRemaining < keypoint1 && cruiseElroyMode == 0 && ghostsFree == 4)
		{
			cruiseElroyMode = 1;
			ghosts[0].speed *= 1.05;
		}
		if (dotsRemaining < keypoint2 && cruiseElroyMode == 1)
		{
			cruiseElroyMode = 2;
			ghosts[0].speed *= 1.05;
		}
	}

	if (242 - dotsRemaining < 80 && ghosts[2].delay == 1 && level == 1) {
		ghosts[2].delay = 2;
	}

	setTargets(mode, playerPos, playerDir, dotsRemaining);

	for (int i = 0; i < 4; i++) {
		ghosts[i].update();
	}
}

void GhostManager::switchMode(Modes m, vec2 playerPos, vec2 playerDir, int dotsRemaining) {
	if (mode == SCATTERING || mode == CHASE) {
		for (int i = 0; i < 4; i++) {
			if (ghosts[i].free && ghosts[i].alive) ghosts[i].dir = -ghosts[i].dir;
		}
	}

	if (mode == CHASE && m == SCATTERING) {
		wave++;
		setDurations();
	}

	if (mode == FRIGHTENED) {
		for (int i = 0; i < 4; i++) {
			ghosts[i].frightened = false;
		}
	}

	switch (m) {
	case SCATTERING:
		setTargets(m, playerPos, playerDir, dotsRemaining);

		switchCd = scatterModeDuration;

		break;
	case CHASE:
		setTargets(m, playerPos, playerDir, dotsRemaining);

		switchCd = chasingModeDuration;

		break;
	case FRIGHTENED:
		for (int i = 0; i < 4; i++) {
			if (ghosts[i].alive && !ghosts[i].frightened)
			{
				ghosts[i].frightened = true;
			}
		}
		frightenedMode = frightenedModeDuration;
		lastMode = (mode == FRIGHTENED) ? lastMode : mode;
	}

	mode = m;
}

void GhostManager::setTargets(Modes m, vec2 playerPos, vec2 playerDir, int dotsRemaining) {
	switch (m) {
	case SCATTERING:
		if (ghosts[0].alive && !ghosts[0].frightened) {
			if (cruiseElroyMode == 0) ghosts[0].target = vec2(C_SIZE * (map[0].size() - 4), 0);
			else ghosts[0].target = playerPos;
		}
		if (ghosts[1].alive && !ghosts[1].frightened) ghosts[1].target = vec2(C_SIZE * 4, 0);
		if (ghosts[2].alive && !ghosts[2].frightened) ghosts[2].target = vec2(0, C_SIZE * map.size());
		if (ghosts[3].alive && !ghosts[3].frightened) ghosts[3].target = vec2(C_SIZE * map[0].size(), C_SIZE * map.size());

		break;

	case CHASE:
		if (ghosts[0].alive && !ghosts[0].frightened) ghosts[0].target = playerPos;
		if (ghosts[1].alive && !ghosts[1].frightened) {
			if (playerDir == dirs[0]) ghosts[1].target = playerPos + vec2(-C_SIZE * 4, -C_SIZE * 4);
			else ghosts[1].target = playerPos + playerDir * C_SIZE * 4;
		}
		if (ghosts[3].alive && !ghosts[3].frightened) {
			if (242 - dotsRemaining > 30) {
				vec2 line = playerPos + playerDir * C_SIZE * 2 - ghosts[0].pos;
				ghosts[3].target = ghosts[0].pos + line * 2;
			}
			else {
				ghosts[3].target = vec2(C_SIZE * map[0].size(), C_SIZE * map.size());
			}
		}
		if (ghosts[2].alive && !ghosts[2].frightened) {
			float dist = length(playerPos - ghosts[2].pos);

			if(dist > 8 * C_SIZE) ghosts[2].target = playerPos;
			else ghosts[2].target = vec2(C_SIZE * map[0].size(), C_SIZE * map.size());
		}

		break;
	}
}

void GhostManager::drawGhosts(const Texture2D& animation) {
	for (int i = 0; i < 4; i++) {
		ghosts[i].draw(animation, frightenedMode);
	}
}
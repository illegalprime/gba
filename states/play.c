#include "play.h"

static u32 wave_number = 0;

static struct Enemies enemies;

static struct Player courage = {
	.box = {
		.x = SCREEN_WIDTH / 2,
		.y = SCREEN_HEIGHT / 2,
		.width = PLAYER_WIDTH,
		.height = PLAYER_HEIGHT,
	},
	.size = {
		.x = PLAYER_WIDTH,
		.y = PLAYER_HEIGHT,
	},
};

i32 clip(i32 target, i32 min, i32 max) {
	if (target < min) {
		target = min;
	}
	else if (target > max) {
		target = max;
	}
	return target;
}

void move(struct BBox* box, i32 x, i32 y) {
	if (x != 0) {
		box->x = clip(box->x + x, 0, SCREEN_WIDTH - box->width);
	}
	if (y != 0) {
		box->y = clip(box->y + y, 0, SCREEN_HEIGHT - box->height);
	}
}

struct Vec2 random_enemy_spawn() {
	// TODO
	return (struct Vec2) {
		.x = 0,
		.y = 0,
	};
}

enum GameState run_play(u32 frame_no) {
	UNUSED(frame_no);
	UNUSED(wave_number);
	UNUSED(enemies);
	// clear buffer by drawing background
	// TODO
	fill(BLACK);

	if (wave_number == 0) {
		// set the count to zero to increase wave number
		enemies.count = 0;
	}

	// handle movement
	struct Buttons events = button_events();
	if (events.held & BUTTON_LEFT) {
		move(&courage.box, -PLAYER_SPEED, 0);
	}
	else if (events.held & BUTTON_RIGHT) {
		move(&courage.box, PLAYER_SPEED, 0);
	}
	if (events.held & BUTTON_UP) {
		move(&courage.box, 0, -PLAYER_SPEED);
	}
	else if (events.held & BUTTON_DOWN) {
		move(&courage.box, 0, PLAYER_SPEED);
	}

	// handle firing
	// TODO

	// handle enemy movement
	// TODO
	bool all_dead = true;
	bool should_spawn = true; // TODO

	for (u32 i = 0; i < enemies.count; i += 1) {
		// if we have an open spot an we need to make a mole
		if (should_spawn && !enemies.moles[i].spawned) {
			// spawn an enemy
			struct Vec2 spawn = random_enemy_spawn();
			enemies.moles[i].spawned = true;
			enemies.moles[i].died = false;
			enemies.moles[i].box.x = spawn.x;
			enemies.moles[i].box.y = spawn.y;
			enemies.moles[i].box.width = ENEMY_WIDTH;
			enemies.moles[i].box.height = ENEMY_HEIGHT;
			should_spawn = false;
		}

		// move it a little bit
		// TODO
		move(&enemies.moles[i].box, 1, 1);

		// check if they're all dead
		if (enemies.moles[i].died == false) {
			all_dead = false;
		}
	}

	// reset the board
	if (all_dead) {
		// increment wave and enemies
		wave_number += 1;
		enemies.count += 1;

		// zero everyone
		for (u32 i = 0; i < enemies.count; i += 1) {
			enemies.moles[i].spawned = false;
		}
	}

	// draw player
	// TODO
	draw_rectangle(
		courage.box.y,
		courage.box.x,
		courage.box.width,
		courage.box.height,
		PURPLE
	);

	// draw enemies
	// TODO
	for (u32 i = 0; i < enemies.count; i += 1) {
		draw_rectangle(
			enemies.moles[i].box.y,
			enemies.moles[i].box.x,
			enemies.moles[i].box.width,
			enemies.moles[i].box.height,
			GREEN
		);
	}
	return GAME_STATE_PLAY;
}


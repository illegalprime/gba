#include "play.h"

#include "../assets/room.h"
#include "../assets/intro_movie.h"
#include "../assets/numbers.h"
#include "../assets/courage.h"

static u32 wave_number = 0;

static struct Enemies enemies;

static struct Bullet bullets[MAX_BULLETS];

static struct Player courage = {
	.box = {
		.x = SCREEN_WIDTH / 2,
		.y = SCREEN_HEIGHT / 2,
		.width = COURAGE_WIDTH,
		.height = COURAGE_HEIGHT,
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

bool collides(struct BBox a, struct BBox b) {
	return a.x < b.x + b.width
		&& a.x + a.width > b.x
		&& a.y < b.y + b.height
		&& a.height + a.y > b.y;
}

void move(struct BBox* box, struct Vec2 vel) {
	if (vel.x != 0) {
		box->x = clip(box->x + vel.x, MOVEMENT_MIN_X, MOVEMENT_MAX_X - box->width);
	}
	if (vel.y != 0) {
		box->y = clip(box->y + vel.y, MOVEMENT_MIN_Y, MOVEMENT_MAX_Y - box->height);
	}
}

struct Vec2 build_vec(struct Vec2 dir, i32 mag) {
	return (struct Vec2) {
		.x = dir.x < 0 ? -mag : (dir.x == 0 ? 0 : mag),
		.y = dir.y < 0 ? -mag : (dir.y == 0 ? 0 : mag),
	};
}

struct Vec2 random_vel(i32 mag) {
	int id_x = rand() % 3;
	int id_y = rand() % 3;
	return (struct Vec2) {
		.x = id_x == 0 ? mag : id_x == 1 ? -mag : 0,
		.y = id_y == 0 ? mag : id_y == 1 ? -mag : 0,
	};
}

struct Vec2 random_enemy_spawn() {
	return (struct Vec2) {
		.x = MOVEMENT_MIN_X
			+ (rand() % (MOVEMENT_MAX_X - MOVEMENT_MIN_X - ENEMY_WIDTH)),
		.y = 30,
	};
}

static bool show_wave = false;
static u16 show_wave_frame = 0;

void draw_wave(u32 wave_number) {
	UNUSED(wave_number);
	draw_image(
		DRAW_WAVE_Y,
		DRAW_WAVE_X,
		WAVE_WIDTH,
		WAVE_HEIGHT,
		wave
	);

	u32 power = 1000000000;
	u32 x = DRAW_WAVE_X + WAVE_WIDTH + digit_widths[0];
	bool leading = true;

	while (wave_number != 0) {
		u32 digit = wave_number / power;
		wave_number = wave_number % power;
		power = power / 10;

		if (leading) {
			if (digit == 0) {
				continue;
			}
			else {
				leading = false;
			}
		}
		draw_image(
			DRAW_WAVE_Y,
			x,
			digit_widths[digit],
			DIGITS_HEIGHT,
			digits[digit]
		);

		// increment the next digit position
		x += digit_widths[digit];
	}
}

enum GameState run_play(u32 frame_no) {
	/*********
	 * LOGIC *
	 *********/
	struct Buttons events = button_events();
	bool game_over = false;

	// show the wave for a little bit, disable controls
	if (show_wave) {
		show_wave_frame += 1;
		if (show_wave_frame == WAVE_PAUSE_TIME) {
			show_wave = false;
			show_wave_frame = 0;
		}

		draw_rectangle(
			MOVEMENT_MIN_Y,
			MOVEMENT_MIN_X,
			MOVEMENT_MAX_X - MOVEMENT_MIN_X,
			MOVEMENT_MAX_Y - MOVEMENT_MIN_Y,
			frame_011446[OFFSET(MOVEMENT_MIN_Y, MOVEMENT_MIN_X)]
		);

		draw_wave(wave_number);

		return GAME_STATE_PLAY;
	}

	srand(frame_no);
	if (wave_number == 0) {
		// set the count to zero to increase wave number
		enemies.count = 0;
		enemies.killed = 0;
		enemies.spawned = 0;
		show_wave = false;
		show_wave_frame = 0;

		// zero all bullets
		for (u32 i = 0; i < MAX_BULLETS; i += 1) {
			bullets[i].spawned = false;
		}

		for (u32 i = 0; i < MAX_WAVES; i += 1) {
			enemies.moles[i].spawned = false;
		}

		fill_image(frame_011446);
	}

	// handle button movement
	if (events.held & BUTTON_LEFT) {
		courage.vel.x = -PLAYER_SPEED;
	}
	else if (events.held & BUTTON_RIGHT) {
		courage.vel.x = PLAYER_SPEED;
	}
	else {
		courage.vel.x = 0;
	}
	if (events.held & BUTTON_UP) {
		courage.vel.y = -PLAYER_SPEED;
	}
	else if (events.held & BUTTON_DOWN) {
		courage.vel.y = PLAYER_SPEED;
	}
	else {
		courage.vel.y = 0;
	}

	// move the player
	move(&courage.box, courage.vel);

	// handle firing
	bool spawn_bullet = false;
	if (events.pressed & BUTTON_A) {
		spawn_bullet = true;
	}

	for (u32 i = 0; i < MAX_BULLETS; i += 1) {
		if (!bullets[i].spawned && spawn_bullet) {
			// FIRE!
			bullets[i].spawned = true;
			bullets[i].box.width = BULLET_WIDTH;
			bullets[i].box.height = BULLET_HEIGHT;
			bullets[i].box.x = courage.box.x + (courage.box.width / 2);
			bullets[i].box.y = courage.box.y + (courage.box.height / 2);

			if (courage.vel.x == 0 && courage.vel.y == 0) {
				bullets[i].vel = build_vec(courage.last_vel, BULLET_SPEED);
			}
			else {
				bullets[i].vel = build_vec(courage.vel, BULLET_SPEED);
			}

			spawn_bullet = false;
		}
		// update the bullets
		if (bullets[i].spawned) {
			bullets[i].box.x += bullets[i].vel.x;
			bullets[i].box.y += bullets[i].vel.y;

			// check if we left and destroy us
			if (bullets[i].box.x <= MOVEMENT_MIN_X
				|| bullets[i].box.x > MOVEMENT_MAX_X - BULLET_WIDTH
				|| bullets[i].box.y <= MOVEMENT_MIN_Y
				|| bullets[i].box.y > MOVEMENT_MAX_Y - BULLET_HEIGHT)
			{
				bullets[i].spawned = false;
				break;
			}

			// check if we hit someone
			for (u32 j = 0; j < enemies.count; j += 1) {
				if (enemies.moles[j].spawned
					&& collides(bullets[i].box, enemies.moles[j].box))
				{
					// they destroy each other
					enemies.moles[j].spawned = false;
					bullets[i].spawned = false;
					enemies.killed += 1;
					break;
				}
			}
		}
	}

	// handle enemy movement
	bool should_spawn = (frame_no % ENEMY_SPAWN_RATE == 0)
		&& (enemies.count > enemies.spawned);

	for (u32 i = 0; i < enemies.count; i += 1) {
		// if we have an open spot an we need to make a mole
		if (should_spawn && !enemies.moles[i].spawned) {
			// spawn an enemy
			struct Vec2 spawn = random_enemy_spawn();
			enemies.moles[i].spawned = true;
			enemies.moles[i].box.x = spawn.x;
			enemies.moles[i].box.y = spawn.y;
			enemies.moles[i].box.width = ENEMY_WIDTH;
			enemies.moles[i].box.height = ENEMY_HEIGHT;
			enemies.moles[i].vel = random_vel(ENEMY_SPEED);
			should_spawn = false;
			enemies.spawned += 1;
		}

		if (enemies.moles[i].spawned) {
			// move it a little bit
			// random chance of changing velocities
			if (frame_no % ENEMY_SPEED == 0) {
				if (rand() % ENEMY_CHANGE_DIR_RATE == 0) {
					enemies.moles[i].vel = random_vel(1);
				}
				move(&enemies.moles[i].box, enemies.moles[i].vel);
			}

			// check if it hit our player
			if (collides(enemies.moles[i].box, courage.box)) {
				game_over = true;
			}
		}
	}

	/*************
	 * RENDERING *
	 *************/
	// clear buffer by drawing background
	draw_rectangle(
		MOVEMENT_MIN_Y,
		MOVEMENT_MIN_X,
		MOVEMENT_MAX_X - MOVEMENT_MIN_X,
		MOVEMENT_MAX_Y - MOVEMENT_MIN_Y,
		frame_011446[OFFSET(MOVEMENT_MIN_Y, MOVEMENT_MIN_X)]
	);

	// draw bullets
	for (u32 i = 0; i < MAX_BULLETS; i += 1) {
		if (bullets[i].spawned) {
			draw_rectangle(
				bullets[i].box.y,
				bullets[i].box.x,
				bullets[i].box.width,
				bullets[i].box.height,
				RED
			);
		}
	}

	// draw player
	draw_image(
		courage.box.y,
		courage.box.x,
		courage.box.width,
		courage.box.height,
		courage_sprite
	);

	// draw enemies
	for (u32 i = 0; i < enemies.count; i += 1) {
		if (enemies.moles[i].spawned) {
			draw_rectangle(
				enemies.moles[i].box.y,
				enemies.moles[i].box.x,
				enemies.moles[i].box.width,
				enemies.moles[i].box.height,
				BROWN
			);
		}
	}

	// do cleanup
	if (courage.vel.x != 0 || courage.vel.y != 0) {
		courage.last_vel = courage.vel;
	}

	// reset the board
	if (enemies.killed == enemies.count) {
		// increment wave and enemies
		wave_number += 1;
		enemies.count += 1;
		enemies.killed = 0;
		enemies.spawned = 0;
		show_wave = true;
		show_wave_frame = 0;
	}

	if (game_over) {
		// reset the game
		wave_number = 0;
		// go to game over screen
		return GAME_STATE_GAME_OVER;
	}

	return GAME_STATE_PLAY;
}


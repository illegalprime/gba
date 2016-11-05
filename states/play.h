/**
 * @author michael
 * play.h
 */

#ifndef PLAY_H
#define PLAY_H

#include "states.h"
#include "../myLib.h"

#define PLAYER_SPEED 1
#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 10

#define ENEMY_WIDTH 10
#define ENEMY_HEIGHT 10

#define MAX_WAVES 64

struct Vec2 {
	i32 x;
	i32 y;
};

struct BBox {
    i32 x;
    i32 y;
    i32 width;
    i32 height;
};

struct Weremole {
	struct BBox box;
	struct Vec2 vel;
    bool spawned;
    bool died;
};

struct Enemies {
	struct Weremole moles[64];
	u16 count;
};

struct Player {
	struct BBox box;
    struct Vec2 size;
};

enum GameState run_play(u32 frame_no);

#endif /* !PLAY_H */

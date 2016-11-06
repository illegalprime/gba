/**
 * @author michael
 * play.h
 */

#ifndef PLAY_H
#define PLAY_H

#include "stdlib.h"
#include "states.h"
#include "../myLib.h"

#define DRAW_WAVE_Y 74
#define DRAW_WAVE_X 70
#define WAVE_PAUSE_TIME 100

#define MOVEMENT_MIN_X 50
#define MOVEMENT_MIN_Y 30
#define MOVEMENT_MAX_X 184
#define MOVEMENT_MAX_Y 136

#define PLAYER_SPEED 1

#define ENEMY_WIDTH 10
#define ENEMY_HEIGHT 10
#define ENEMY_SPEED 2
#define ENEMY_CHANGE_DIR_RATE 25
#define ENEMY_SPAWN_RATE 200

#define BULLET_SPEED 2
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5

#define MAX_WAVES 64
#define MAX_BULLETS 64

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
};

struct Enemies {
    struct Weremole moles[64];
    u16 count;
    u16 spawned;
    u16 killed;
};

struct Bullet {
    struct BBox box;
    struct Vec2 vel;
    bool spawned;
};

struct Player {
    struct BBox box;
    struct Vec2 vel;
    struct Vec2 last_vel;
};

enum GameState run_play(u32 frame_no);

#endif /* !PLAY_H */

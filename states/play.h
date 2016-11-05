/**
 * @author michael
 * play.h
 */

#ifndef PLAY_H
#define PLAY_H

#include "stdlib.h"
#include "states.h"
#include "../myLib.h"

#define PLAYER_SPEED 1
#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 10

#define ENEMY_WIDTH 10
#define ENEMY_HEIGHT 10
#define ENEMY_SPEED 1
#define ENEMY_CHANGE_DIR_RATE 25
#define ENEMY_SPAWN_RATE 200

#define BULLET_SPEED 2
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 2

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
    struct Vec2 size;
    struct Vec2 vel;
    struct Vec2 last_vel;
};

enum GameState run_play(u32 frame_no);

#endif /* !PLAY_H */

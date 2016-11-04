/**
 * @author michael
 * states.h
 */

#ifndef STATES_H
#define STATES_H

#include "../myLib.h"

#define GAME_STATE_START GAME_STATE_MENU

enum GameState {
    GAME_STATE_MENU,
    GAME_STATE_INTRO,
    GAME_STATE_PLAY,
};

enum GameState eval_state(enum GameState current, u32 frame_no);

#endif /* !STATES_H */

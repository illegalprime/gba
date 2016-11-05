#include "states.h"

/* states */
#include "menu.h"
#include "intro.h"
#include "play.h"
#include "game_over.h"

enum GameState eval_state(enum GameState current, u32 frame_no) {
	switch (current) {
		case GAME_STATE_MENU:
			return run_menu(frame_no);
		case GAME_STATE_INTRO:
			return run_intro(frame_no);
		case GAME_STATE_PLAY:
			return run_play(frame_no);
		case GAME_STATE_GAME_OVER:
			return run_game_over(frame_no);
		default:
			return current;
	}
}

#include "states.h"

/* states */
#include "menu.h"
#include "intro.h"

enum GameState eval_state(enum GameState current, u32 frame_no) {
	switch (current) {
		case GAME_STATE_MENU:
			return run_menu(frame_no);
		case GAME_STATE_INTRO:
			return run_intro(frame_no);
		default:
			return current;
	}
}

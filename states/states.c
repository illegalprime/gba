#include "states.h"

/* states */
#include "menu.h"

enum GameState eval_state(enum GameState current, u32 frame_no) {
	switch (current) {
		case GAME_STATE_MENU:
			return run_menu(frame_no);
		default:
			return current;
	}
}

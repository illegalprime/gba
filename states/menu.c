#include "menu.h"

// assets
#include "../assets/splash.h"
#include "../assets/splash-press-start.h"

enum GameState run_menu(u32 frame_no) {
	UNUSED(frame_no);
	if ((frame_no / 100) % 2 == 0) {
		fill_image(splash);
	}
	else {
		fill_image(splashpressstart);
	}
	return GAME_STATE_MENU;
}


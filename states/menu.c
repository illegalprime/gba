#include "menu.h"

// assets
#include "../assets/splash.h"
#include "../assets/splash-press-start.h"

enum GameState run_menu(u32 frame_no) {
	// loop between start menus
	if ((frame_no / 100) % 2 == 0) {
		fill_image(splash);
	}
	else {
		fill_image(splashpressstart);
	}

	// check if the start button is pressed
	struct Buttons events = button_events();
	if (events.pressed & BUTTON_START) {
		return GAME_STATE_INTRO;
	}
	return GAME_STATE_MENU;
}


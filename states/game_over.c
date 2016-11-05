#include "game_over.h"

u32 start_time = 0;
u32 game_over_last_frame = 0;

enum GameState run_game_over(u32 frame_no) {
	if (frame_no != game_over_last_frame + 1) {
		// reset
		start_time = 0;
	}
	if (start_time == 0) {
		// make all the pixels more red
		for (u16 i = 0; i < SCREEN_WIDTH; i += 1) {
			for (u16 j = 0; j < SCREEN_HEIGHT; j += 1) {
				u16 color = get_pixel(j, i);
				set_pixel(j, i, color | 0xf);
			}
		}
	}
	if (start_time == GAME_OVER_DELAY_TIME) {
		// draw the game over screen
		fill_image(gameover);
	}
	else {
		start_time += 1;
	}
	game_over_last_frame = frame_no;
	return GAME_STATE_GAME_OVER;
}

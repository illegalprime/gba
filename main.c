// Make a simple GBA state machine
// Name: Michael Eden

#include "myLib.h"
#include "states/states.h"

int main() {
	REG_DISPCNT = MODE_3 | BG2_EN;
	enum GameState state = GAME_STATE_MENU;

	u32 frame_id = 0;

	// wait for v-sync
	while (wait_for_vblank()) {
		// run whatever state we're in
		state = eval_state(state, frame_id);

		// increment frame
		frame_id += 1;
	}

	return 0;
}


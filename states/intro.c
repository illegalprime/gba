/*
 * intro.c
 * Copyright (C) 2016 michael <themichaeleden@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "intro.h"

enum GameState run_intro(u32 frame_no) {
	UNUSED(frame_no);
	fill(PURPLE);
	return GAME_STATE_INTRO;
}


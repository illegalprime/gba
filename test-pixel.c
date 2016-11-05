#include "./assets/intro_movie.h"

#include "stdio.h"

#define OFFSET(y, x) ((y * 240) + x)

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;
	int y = 26;
	for (int i = 76; i < 145; i += 1) {
		printf("%x\n", frame_011446[OFFSET(y, i)]);
	}
}


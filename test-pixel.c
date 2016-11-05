#include "./assets/intro_movie.h"

#include "stdio.h"

#define OFFSET(y, x) ((y * 240) + x)

int main(int argc, char** argv) {
	printf("%x\n", frame_011446[30 * 240 + 50]);
	printf("%x\n", frame_011446[30 * 240 + 49]);
}


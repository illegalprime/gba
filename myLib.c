#include "myLib.h"

u16* video_buffer = (u16*) 0x6000000;

struct Buttons btn_events;

// General DMA fill routine
static inline void dma_fill(void *dst, volatile u32 src, u32 count) {
    REG_DMA[3].cnt = 0; // shut off any previous transfer
    REG_DMA[3].src = (const void*) &src;
    REG_DMA[3].dst = dst;
    REG_DMA[3].cnt = count | DMA_SRC_FIXED | DMA_ON;
}

// General DMA copier
static inline void dma_copy(void *dst, const void *src, u32 count) {
    REG_DMA[3].cnt = 0; // shut off any previous transfer
    REG_DMA[3].src = src;
    REG_DMA[3].dst = dst;
    REG_DMA[3].cnt = count | DMA_ON;
}

int wait_for_vblank() {
	// wait for sync
	while (REG_DISPLAY_VCOUNT >= 160);
	while (REG_DISPLAY_VCOUNT < 160);

	// get button events
	btn_events.pressed = ~btn_events.held & BUTTONS;
	btn_events.held = BUTTONS;

	return 1;
}

void set_pixel(int row, int col, u16 color) {
	if (OFFSET(row, col) < 240 * 160) {
		video_buffer[OFFSET(row, col)] = color;
	}
}

void draw_rectangle(int row, int col, int width, int height, u16 color) {
	for (int i = 0; i < height; i += 1) {
		for (int j = 0; j < width; j += 1) {
			set_pixel(i + row, j + col, color);
		}
	}
}

void draw_hollow_rectangle(int row, int col, int width, int height, u16 color) {
	for (int i = 0; i < width; i += 1) {
		set_pixel(row, col + i, color);
		set_pixel(row + height - 1, col + i, color);
	}
	for (int i = 0; i < height; i += 1) {
		set_pixel(i + row, col, color);
		set_pixel(i + row, col + width - 1, color);
	}
}

void fill(u32 color) {
	dma_fill(video_buffer, color, SCREEN_SIZE);
}

void fill_image(const unsigned short* image) {
	dma_copy(video_buffer, image, SCREEN_SIZE);
}

struct Buttons button_events() {
	return btn_events;
}

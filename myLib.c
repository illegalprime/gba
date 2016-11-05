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
	u16 btns = ~(BUTTONS);
	btn_events.pressed = ~btn_events.held & btns;
	btn_events.held = btns;

	return 1;
}

void set_pixel(int row, int col, u16 color) {
	if (OFFSET(row, col) < 240 * 160) {
		video_buffer[OFFSET(row, col)] = color;
	}
}

u16 get_pixel(int row, int col) {
	if (OFFSET(row, col) < 240 * 160) {
		return video_buffer[OFFSET(row, col)];
	}
	return 0;
}

void draw_rectangle(int row, int col, int width, int height, u16 color) {
    for (int i = 0; i < height; i += 1) {
        dma_fill(video_buffer + OFFSET(row + i, col), color, width);
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

void draw_image(u8 row, u8 col, u8 width, u8 height, const u16* image) {
    for (u8 i = 0; i < height; i += 1) {
        dma_copy(video_buffer + OFFSET(row + i, col), image + (i * width), width);
    }
}

struct Buttons button_events() {
	return btn_events;
}

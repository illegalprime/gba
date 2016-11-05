/**
 * @author Michael Eden
 * myLib.h
 */

#ifndef MYLIB_H
#define MYLIB_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char bool;
typedef char i8;
typedef short i16;
typedef int i32;

#define true 1
#define false 0
#define NULL 0
#define UNUSED(x) (void)(x)

// Useful defines
#define MEM_IO 0x4000000
#define REG_DISPCNT (*(unsigned short*) MEM_IO)
#define REG_DISPLAY_VCOUNT (*((unsigned short*)(MEM_IO + 0x0006)))
#define MODE_3 3
#define BG2_EN (1 << 10)

#define COLOR(R,G,B) (((R) & 0x1F) | (((G) & 0x1F) << 5) | (((B) & 0x1F) << 10))
#define BLACK   COLOR(0 , 0 , 0 )
#define WHITE   COLOR(31, 31, 31)
#define RED     COLOR(31, 0 , 0 )
#define GREEN   COLOR(0 , 31, 0 )
#define BLUE    COLOR(0 , 0 , 31)
#define YELLOW  COLOR(31, 31, 0 )
#define MAGENTA COLOR(31, 0 , 31)
#define CYAN    COLOR(0 , 31, 31)
#define ORANGE  COLOR(31, 15, 0 )
#define BROWN   COLOR(18, 9 , 0 )
#define PURPLE  COLOR(15, 0 , 15)
#define TEAL    COLOR(0 , 15, 15)
#define MAROON  COLOR(15, 0 , 0 )
#define GREY    COLOR(15, 15, 15)
#define PINK    COLOR(31, 18, 19)

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define SCREEN_SIZE (240 * 160)

#define BUTTONS       *(volatile unsigned short*) 0x4000130
#define BUTTON_A        0x0001
#define BUTTON_B        0x0002
#define BUTTON_SELECT   0x0004
#define BUTTON_START    0x0008
#define BUTTON_RIGHT    0x0010
#define BUTTON_LEFT     0x0020
#define BUTTON_UP       0x0040
#define BUTTON_DOWN     0x0080
#define BUTTON_R        0x0100
#define BUTTON_L        0x0200
#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

#define OFFSET(r,c) ((r)*240+(c))

/* Destination address flags */
#define DMA_DEST_INC (0 << 21)
#define DMA_DEST_DEC (1 << 21)
#define DMA_DEST_FIXED (2 << 21)
#define DMA_DEST_RESET (3 << 21)

/* Source address flags */
#define DMA_SRC_INC (0 << 23)
#define DMA_SRC_DEC (1 << 23)
#define DMA_SRC_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

/* How much to copy flags */
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

/* When to DMA flags */
#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)

#define DMA_ON (1 << 31)

typedef struct DMA_REC {
    const void *src;
    void *dst;
    u32 cnt;
} DMA_REC;

#define REG_DMA ((volatile DMA_REC*)0x040000B0)

struct Buttons {
	u16 held;
	u16 pressed;
};

extern unsigned short *video_buffer;
extern u32 frame_id;

int wait_for_vblank();

void set_pixel(int row, int col, u16 color);

u16 get_pixel(int row, int col);

void draw_rectangle(int row, int col, int width, int height, u16 color);

void draw_hollow_rectangle(int row, int col, int width, int height, u16 color);

void fill(u32 color);

void fill_image(const unsigned short* image);

void draw_image(u8 row, u8 col, u8 width, u8 height, const u16* image);

struct Buttons button_events();

#endif /* !MYLIB_H */

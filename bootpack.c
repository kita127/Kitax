#include "./dsctbl/dsctbl.h"
#include "./graphic/graphic.h"
#include "./naskfunc/naskfunc.h"

typedef struct {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
} BOOTINFO;

void boxfill8(char *vram, int xsize, unsigned char color, int x_s, int y_s,
              int x_e, int y_e);
void init_screen(char *vram, int xsize, int ysize);
void putfont8(char *vram, short xsize, int x, int y, char color, char font[]);
void putfonts8_asc(char *vram, short xsize, int x, int y, char color, char s[]);

void mysprintf(char *str, char *fmt, ...);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char vram[], short vxsize, int pxsize, int pysize, int px,
                 int py, char buf[], int bxsize);

#define COL8_000000 (0)  /*  0:黒 */
#define COL8_FF0000 (1)  /*  1:明るい赤 */
#define COL8_00FF00 (2)  /*  2:明るい緑 */
#define COL8_FFFF00 (3)  /*  3:明るい黄色 */
#define COL8_0000FF (4)  /*  4:明るい青 */
#define COL8_FF00FF (5)  /*  5:明るい紫 */
#define COL8_00FFFF (6)  /*  6:明るい水色 */
#define COL8_FFFFFF (7)  /*  7:白 */
#define COL8_C6C6C6 (8)  /*  8:明るい灰色 */
#define COL8_840000 (9)  /*  9:暗い赤 */
#define COL8_008400 (10) /* 10:暗い緑 */
#define COL8_848400 (11) /* 11:暗い黄色 */
#define COL8_000084 (12) /* 12:暗い青 */
#define COL8_840084 (13) /* 13:暗い紫 */
#define COL8_008484 (14) /* 14:暗い水色 */
#define COL8_848484 (15) /* 15:暗い灰色 */

void HariMain(void) {
    BOOTINFO *binfo = (BOOTINFO *)0x0ff0;
    char s[64];
    int mx, my;
    char mcursor[16 * 16];

    init_gdtidt();
    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    mysprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

    for (;;) {
        io_hlt();
    }
}

void boxfill8(char *vram, int xsize, unsigned char color, int x_s, int y_s,
              int x_e, int y_e) {
    int x, y;
    for (y = y_s; y <= y_e; y++) {
        for (x = x_s; x <= x_e; x++) {
            vram[y * xsize + x] = color;
        }
    }
}

void init_screen(char *vram, int xsize, int ysize) {
    boxfill8(vram, xsize, COL8_008484, 0, 0, xsize - 1, ysize - 29);
    boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize - 28, xsize - 1, ysize - 28);
    boxfill8(vram, xsize, COL8_FFFFFF, 0, ysize - 27, xsize - 1, ysize - 27);
    boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize - 26, xsize - 1, ysize - 1);

    boxfill8(vram, xsize, COL8_FFFFFF, 3, ysize - 24, 59, ysize - 24);
    boxfill8(vram, xsize, COL8_FFFFFF, 2, ysize - 24, 2, ysize - 4);
    boxfill8(vram, xsize, COL8_848484, 3, ysize - 4, 59, ysize - 4);
    boxfill8(vram, xsize, COL8_848484, 59, ysize - 23, 59, ysize - 5);
    boxfill8(vram, xsize, COL8_000000, 2, ysize - 3, 59, ysize - 3);
    boxfill8(vram, xsize, COL8_000000, 60, ysize - 24, 60, ysize - 3);

    boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize - 4,
             ysize - 24);
    boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47,
             ysize - 4);
    boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize - 3, xsize - 4,
             ysize - 3);
    boxfill8(vram, xsize, COL8_FFFFFF, xsize - 3, ysize - 24, xsize - 3,
             ysize - 3);
}

void putfont8(char *vram, short xsize, int x, int y, char color, char font[]) {
    int i;
    char *p;
    char d;

    for (i = 0; i < 16; i++) {
        p = vram + (y + i) * xsize + x;
        d = font[i];
        if ((d & 0x80) != 0x00) {
            p[0] = color;
        }
        if ((d & 0x40) != 0x00) {
            p[1] = color;
        }
        if ((d & 0x20) != 0x00) {
            p[2] = color;
        }
        if ((d & 0x10) != 0x00) {
            p[3] = color;
        }
        if ((d & 0x08) != 0x00) {
            p[4] = color;
        }
        if ((d & 0x04) != 0x00) {
            p[5] = color;
        }
        if ((d & 0x02) != 0x00) {
            p[6] = color;
        }
        if ((d & 0x01) != 0x00) {
            p[7] = color;
        }
    }
}

void putfonts8_asc(char *vram, short xsize, int x, int y, char color,
                   char s[]) {
    extern char hankaku[4096];
    for (; *s != '\0'; s++) {
        putfont8(vram, xsize, x, y, color, hankaku + *s * 16);
        x += 8;
    }
}

void init_mouse_cursor8(char *mouse, char bc)
/* マウスカーソルを準備（16x16） */
{
    static char cursor[16][16] = {
        "**************..", "*OOOOOOOOOOO*...", "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....", "*OOOOOOOO*......", "*OOOOOOO*.......",
        "*OOOOOOO*.......", "*OOOOOOOO*......", "*OOOO**OOO*.....",
        "*OOO*..*OOO*....", "*OO*....*OOO*...", "*O*......*OOO*..",
        "**........*OOO*.", "*..........*OOO*", "............*OO*",
        ".............***"};
    int x, y;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = COL8_000000;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = COL8_FFFFFF;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = bc;
            }
        }
    }
    return;
}

void putblock8_8(char vram[], short vxsize, int pxsize, int pysize, int px,
                 int py, char buf[], int bxsize) {

    int x, y;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            vram[(py + y) * vxsize + (px + x)] = buf[y * bxsize + x];
        }
    }
}

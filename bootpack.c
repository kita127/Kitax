void init_palette(void);
void set_palette(int start, int end, unsigned char rgb[]);
void boxfill8(char *vram, int xsize, unsigned char color, int x_s, int y_s,
              int x_e, int y_e);
void init_screen(char *vram, int xsize, int ysize);
void putfont8(char *vram, short xsize, int x, int y, char color, char font[]);
void putfonts8_asc(char *vram, short xsize, int x, int y, char color, char s[]);

void mysprintf(char *str, char *fmt, ...);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char vram[], short vxsize, int pxsize, int pysize, int px,
                 int py, char buf[], int bxsize);

/* naskfunc */
void io_hlt(void);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void io_out8(int port, char data);
void io_cli(void);

#define PALETTE_NUM (16)

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

typedef struct {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
} BOOTINFO;

void HariMain(void) {
    BOOTINFO *binfo = (BOOTINFO *)0x0ff0;
    char s[64];
    int mx, my;
    char mcursor[16 * 16];

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

void init_palette(void) {

    static unsigned char table_rgb[PALETTE_NUM * 3] = {
        0x00, 0x00, 0x00, /*  0:黒 */
        0xff, 0x00, 0x00, /*  1:明るい赤 */
        0x00, 0xff, 0x00, /*  2:明るい緑 */
        0xff, 0xff, 0x00, /*  3:明るい黄色 */
        0x00, 0x00, 0xff, /*  4:明るい青 */
        0xff, 0x00, 0xff, /*  5:明るい紫 */
        0x00, 0xff, 0xff, /*  6:明るい水色 */
        0xff, 0xff, 0xff, /*  7:白 */
        0xc6, 0xc6, 0xc6, /*  8:明るい灰色 */
        0x84, 0x00, 0x00, /*  9:暗い赤 */
        0x00, 0x84, 0x00, /* 10:暗い緑 */
        0x84, 0x84, 0x00, /* 11:暗い黄色 */
        0x00, 0x00, 0x84, /* 12:暗い青 */
        0x84, 0x00, 0x84, /* 13:暗い紫 */
        0x00, 0x84, 0x84, /* 14:暗い水色 */
        0x84, 0x84, 0x84  /* 15:暗い灰色 */
    };

    set_palette(0, (PALETTE_NUM - 1), table_rgb);
}

void set_palette(int start, int end, unsigned char rgb[]) {
    int i;
    int eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        /* n / 4 している理由は不明 */
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
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

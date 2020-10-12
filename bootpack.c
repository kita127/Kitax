#include "./bootpack.h"
#include "./dsctbl/dsctbl.h"
#include "./graphic/graphic.h"
#include "./int/int.h"
#include "./lib/lib.h"
#include "./naskfunc/naskfunc.h"

static void putfont8(char *vram, short xsize, int x, int y, char color,
                     char font[]);

void HariMain(void) {
    BOOTINFO *binfo = (BOOTINFO *)ADR_BOOTINFO;
    char s[64];
    int mx, my;
    char mcursor[16 * 16];

    init_gdtidt();
    init_pic();
    /* IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除 */
    io_sti();

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    mysprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

    io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
    io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

    for (;;) {
        io_hlt();
    }
}

static void putfont8(char *vram, short xsize, int x, int y, char color,
                     char font[]) {
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

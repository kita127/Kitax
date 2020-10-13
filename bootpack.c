#include "./bootpack.h"
#include "./dsctbl/dsctbl.h"
#include "./graphic/graphic.h"
#include "./int/int.h"
#include "./lib/lib.h"
#include "./naskfunc/naskfunc.h"

void HariMain(void) {
    BOOTINFO *binfo = (BOOTINFO *)ADR_BOOTINFO;
    char s[64];
    int mx, my;
    char mcursor[16 * 16];
    unsigned char keydata;

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
        io_cli();
        if (keybuf.has_notice == 0) {
            io_stihlt();
        } else {
            keydata = keybuf.data;
            keybuf.has_notice = 0;
            io_sti();
            mysprintf(s, "%x", keydata);
            boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
            putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
        }
    }
}

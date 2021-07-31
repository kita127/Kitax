#include "./bootpack.h"
#include "./dsctbl/dsctbl.h"
#include "./fifo/fifo.h"
#include "./graphic/graphic.h"
#include "./int/int.h"
#include "./lib/lib.h"
#include "./naskfunc/naskfunc.h"

#define KEYBUF_NUM        (32)
#define MOUSE_NUM         (128)
#define PORT_KEYSTA       (0x0064)
#define PORT_KEYCMD       (0x0064)
#define PORT_KEYDAT       (0x0060)
#define KBC_MODE          (0x47)
#define KEYCMD_WRITE_MODE (0x60)

#define KEYSTA_SEND_NOTREADY (0x0002)
#define KEYCMD_SENDTO_MOUSE  (0xd4)
#define MOUSECMD_ENABLE      (0xf4)

static void wait_KBC_sendready(void);
static void init_keyboard(void);
static void enable_mouse(void);

void HariMain(void) {
    BOOTINFO *binfo = (BOOTINFO *)ADR_BOOTINFO;
    char s[64];
    int mx, my;
    char mcursor[16 * 16];
    unsigned char data;
    unsigned char keybuf[KEYBUF_NUM];
    unsigned char mousebuf[MOUSE_NUM];
    unsigned char mouse_dbuf[3];
    unsigned char mouse_phase;

    init_gdtidt();
    init_pic();
    /* IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除 */
    io_sti();

    fifo8_init(&keyfifo, KEYBUF_NUM, keybuf);
    fifo8_init(&mousefifo, MOUSE_NUM, mousebuf);
    io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
    io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

    init_keyboard();

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    mysprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

    enable_mouse();
    mouse_phase = 0;

    for (;;) {
        io_cli();
        if (fifo8_data_count(&keyfifo) + fifo8_data_count(&mousefifo) == 0) {
            io_stihlt();
        } else {
            if (fifo8_data_count(&keyfifo) != 0) {
                data = fifo8_get(&keyfifo);
                io_sti();
                mysprintf(s, "%x", data);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
                putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
            } else if (fifo8_data_count(&mousefifo) != 0) {
                data = fifo8_get(&mousefifo);
                io_sti();
                if (mouse_phase == 0) {
                    /* マウスの0xfaを待っている段階 */
                    if (data == 0xfa) {
                        mouse_phase = 1;
                    }
                } else if (mouse_phase == 1) {
                    /* マウスの1バイト目を待っている段階 */
                    mouse_dbuf[0] = data;
                    mouse_phase = 2;
                } else if (mouse_phase == 2) {
                    /* マウスの2バイト目を待っている段階 */
                    mouse_dbuf[1] = data;
                    mouse_phase = 3;
                } else if (mouse_phase == 3) {
                    /* マウスの3バイト目を待っている段階 */
                    mouse_dbuf[2] = data;
                    mouse_phase = 1;
                    /* データが3バイト揃ったので表示 */
                    mysprintf(s, "%x %x %x", mouse_dbuf[0], mouse_dbuf[1],
                              mouse_dbuf[2]);
                    boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16,
                             32 + 8 * 8 - 1, 31);
                    putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16,
                                  COL8_FFFFFF, s);
                }
            }
        }
    }
}

static void wait_KBC_sendready(void) {
    /* キーボードコントローラーがデータ送信可能になるのをまつ */
    while (1) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0x0000) {
            break;
        }
    }
}

static void init_keyboard(void) {
    /* キーボードコントローラの初期化 */
    /* マウス制御回路もキーボード制御回路にある */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
}

static void enable_mouse(void) {
    /* マウス有効化 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);

    /* 問題なく初期化されれば0xfa が送信される */
}

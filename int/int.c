#include "./int.h"
#include "../bootpack.h"
#include "../graphic/graphic.h"
#include "../naskfunc/naskfunc.h"

#define EDGE_TRIG_MODE (0x11)
#define NONE_BUF_MODE  (0x01)

/*
PIC : programmable interrupt controler
      PIC0 の IRQ2 ピン？に PIC1 が接続されている

IMR : interrupt mask register
    : 割り込みを有効にしたい箇所のビットを 0
    : 向こうにしたい箇所ビットを 1

ICW : inital control word
    : PIC の初期設定をするレジスタ

*/
void init_pic(void) {

    /* 初期化前に全ての割り込みをマスクする設定にする */
    io_out8(PIC0_IMR, 0xff);
    io_out8(PIC1_IMR, 0xff);

    /* --- PIC0(Master) の設定 --- */
    io_out8(PIC0_ICW1, EDGE_TRIG_MODE);
    /* IRQ0-7 は、INT20−27 で受ける */
    io_out8(PIC0_ICW2, 0x20);
    /* PIC1 は IRQ2 にて接続する */
    io_out8(PIC0_ICW3, 1 << 2);
    io_out8(PIC0_ICW4, NONE_BUF_MODE);

    /* --- PIC1(Slave) の設定 --- */
    io_out8(PIC1_ICW1, EDGE_TRIG_MODE);
    /* IRQ8-15 は、INT28−2f で受ける */
    io_out8(PIC1_ICW2, 0x28);
    /* PIC1 は IRQ2 にて接続する */
    io_out8(PIC1_ICW3, 2);
    io_out8(PIC1_ICW4, NONE_BUF_MODE);

    /* PIC1 のみ割り込みを有効に設定 */
    io_out8(PIC0_IMR, 0xfb);
    io_out8(PIC1_IMR, 0xff);
}

/* PS/2キーボードからの割り込み */
void inthandler21(int *esp) {
    BOOTINFO *binfo = (BOOTINFO *)ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF,
                  "INT 21 (IRQ-1) : PS/2 keyboard");
    while (1) {
        io_hlt();
    }
}

/* PS/2マウスからの割り込み */
void inthandler2c(int *esp) {
    BOOTINFO *binfo = (BOOTINFO *)ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF,
                  "INT 2C (IRQ-1) : PS/2 mouse");
    while (1) {
        io_hlt();
    }
}

/*
PIC0からの不完全割り込み対策
Athlon64X2機などではチップセットの都合によりPICの初期化時にこの割り込みが1度だけおこる
この割り込み処理関数は、その割り込みに対して何もしないでやり過ごす
なぜ何もしなくていいの？
    →  この割り込みはPIC初期化時の電気的なノイズによって発生したものなので、
        まじめに何か処理してやる必要がない。
*/
void inthandler27(int *esp) {

    /* IRQ-07受付完了をPICに通知(7-1参照) */
    io_out8(PIC0_OCW2, 0x67);
}

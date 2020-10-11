#include "../naskfunc/naskfunc.h"

#define PIC0_ICW1 (0x0020)
#define PIC0_IMR  (0x0021)
#define PIC0_ICW2 (0x0021)
#define PIC0_ICW3 (0x0021)
#define PIC0_ICW4 (0x0021)
#define PIC1_ICW1 (0x00a0)
#define PIC1_IMR  (0x00a1)
#define PIC1_ICW2 (0x00a1)
#define PIC1_ICW3 (0x00a1)
#define PIC1_ICW4 (0x00a1)

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

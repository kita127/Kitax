#include "./int.h"
#include "../bootpack.h"
#include "../fifo/fifo.h"
#include "../graphic/graphic.h"
#include "../lib/lib.h"
#include "../naskfunc/naskfunc.h"

#define EDGE_TRIG_MODE (0x11)
#define NONE_BUF_MODE  (0x01)

#define PORT_KEYDAT (0x0060)

/* IRQ 番号に 0x60 を加算した値 */
/* PIC0 IRQ0 - IRQ7 */
#define IRQ01 (0x61)
#define IRQ02 (0x62)

/* PIC1 IRQ8 - IRQ15 */
#define IRQ12 (0x64)

FIFO8 keyfifo;
FIFO8 mousefifo;

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
    unsigned char data;

    /* IRQ-01受付完了をPICに通知 */
    io_out8(PIC0_OCW2, IRQ01);
    data = io_in8(PORT_KEYDAT);
    fifo8_put(&keyfifo, data);
}

/* PS/2マウスからの割り込み */
void inthandler2c(int *esp) {
    unsigned char data;

    /* IRQ-12 受付完了を PIC1 に通知 */
    io_out8(PIC1_OCW2, IRQ12);

    /* IRQ-02 受付完了を PIC0 に通知 */
    /* IRQ-02 スレーブがある */
    io_out8(PIC0_OCW2, IRQ02);

    /* マウスのデータもキーボードのポートから取得する */
    data = io_in8(PORT_KEYDAT);
    fifo8_put(&mousefifo, data);
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

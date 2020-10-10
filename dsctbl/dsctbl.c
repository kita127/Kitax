#include "../naskfunc/naskfunc.h"

typedef struct {
    short limit_low;
    short base_low;
    char base_mid;
    char access_right;
    char limit_high;
    char base_high;
} SEGMENT_DESCRIPTOR;

typedef struct {
    short offset_low;
    short selector;
    char dw_count;
    char access_right;
    short offset_high;
} GATE_DESCRIPTOR;

#define SET_D ((short)(0x4000U))

#define UNUSED_DESC ((short)0x0000U)
#define SYS_WR      ((short)0x0092U)
#define SYS_X       ((short)0x009aU)

/* *** プロトタイプ宣言 *** */
static void set_segmdesc(SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base,
                         int ar);
static void set_gatedesc(GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

/* *** 関数定義 *** */
void init_gdtidt(void) {
    SEGMENT_DESCRIPTOR *gdt = (SEGMENT_DESCRIPTOR *)0x00270000;
    GATE_DESCRIPTOR *idt = (GATE_DESCRIPTOR *)0x0026f800;
    int i;

    /* GDT の初期化 */
    for (i = 0; i < 8192; i++) {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, (SET_D | SYS_WR));
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, (SET_D | SYS_X));
    load_gdtr(0xffff, 0x00270000);

    /* IDT の初期化 */
    for (i = 0; i < 256; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(0x7ff, 0x0026f800);
}

/* ar : セグメント属性                                            */
/*     bit 長 : 16                                                */
/*     上位 4 bit は「拡張アクセス権」であり 3 bit 目は           */
/*     G フラグ 2bit 目はセグメントモード 0 は 16ビットモード     */
/*     1 は 32bit モードを表す                                    */
/*     セグメント属性は 12 bit だが 16 bit で用意し               */
/*     xxxx0000xxxxxxxx の様に上位                                */
/*     4 bit と下位 8 bit                                         */
/*                                                                */
/*     この辺全般の詳細は書籍の 123P 6−4 「やり残した説明」を参照 */

static void set_segmdesc(SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base,
                         int ar) {
    if (limit > 0xfffff) {
        /* G bit を ON にし limit をページ単位(4K)にする */
        ar |= 0x8000; /* G_bit = 1 */
        /* 4K 単位にするため 4096 で割る */
        limit /= 0x1000;
    }
    sd->limit_low = limit & 0xffff;
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high = (base >> 24) & 0xff;
}

static void set_gatedesc(GATE_DESCRIPTOR *gd, int offset, int selector,
                         int ar) {
    gd->offset_low = offset & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high = (offset >> 16) & 0xffff;
}

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

#define UNUSED_DESC  ((short)0x0000U)
#define SYS_WR       ((short)0x0092U)
#define SYS_X        ((short)0x009aU)
#define LIMIT_BOTPAK (0x0007ffff)
#define ADR_BOTPAK   (0x00280000)
#define LIMIT_GDT    (0x0000ffff)
#define ADR_GDT      (0x00270000)
#define LIMIT_IDT    (0x000007ff)
#define ADR_IDT      (0x0026f800)
/* 割り込み処理に関するマーク */
#define AR_INTGATE32 (0x008e)

/* *** プロトタイプ宣言 *** */
static void set_segmdesc(SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base,
                         int ar);
static void set_gatedesc(GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

/* *** 関数定義 *** */
void init_gdtidt(void) {
    SEGMENT_DESCRIPTOR *gdt = (SEGMENT_DESCRIPTOR *)ADR_GDT;
    GATE_DESCRIPTOR *idt = (GATE_DESCRIPTOR *)ADR_IDT;
    int i;

    /* GDT の初期化 */
    for (i = 0; i <= LIMIT_GDT / 8; i++) {
        set_segmdesc(gdt + i, 0, 0, 0);
    }

    /* 領域全体を表すセグメント */
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, (SET_D | SYS_WR));
    set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, (SET_D | SYS_X));
    load_gdtr(LIMIT_GDT, ADR_GDT);

    /* IDT の初期化 */
    for (i = 0; i <= LIMIT_IDT / 8; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(LIMIT_IDT, ADR_IDT);

    /* IDT の設定 */
    /* 2 * 8 は２番目のセグメントに配置することを表す */
    /* 2番目のセグメントは bootpack.hrb のセグメント */
    set_gatedesc(idt + 0x21, (int)asm_inthandler21, 2 * 8, AR_INTGATE32);
    set_gatedesc(idt + 0x27, (int)asm_inthandler27, 2 * 8, AR_INTGATE32);
    set_gatedesc(idt + 0x2c, (int)asm_inthandler2c, 2 * 8, AR_INTGATE32);
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

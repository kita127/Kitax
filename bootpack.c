#define PALETTE_NUM (16)

void io_hlt(void);
void write_mem8(int addr, int data);
void init_palette(void);
void set_palette(int start, int end, unsigned char rgb[]);

int io_load_eflags(void);
void io_store_eflags(int eflags);
void io_out8(int port, char data);
void io_cli(void);

void HariMain(void) {
    int i;

    init_palette();

    for (i = 0xa0000; i <= 0xaffff; i++) {
        *((char *)i) = i & 0x0f;
    }
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

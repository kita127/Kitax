#ifndef INT_H
#define INT_H

#include "../fifo/fifo.h"

#define PIC0_ICW1 (0x0020)
#define PIC0_OCW2 (0x0020)
#define PIC0_IMR  (0x0021)
#define PIC0_ICW2 (0x0021)
#define PIC0_ICW3 (0x0021)
#define PIC0_ICW4 (0x0021)
#define PIC1_ICW1 (0x00a0)
#define PIC1_OCW2 (0x00a0)
#define PIC1_IMR  (0x00a1)
#define PIC1_ICW2 (0x00a1)
#define PIC1_ICW3 (0x00a1)
#define PIC1_ICW4 (0x00a1)

#define KEYBUF_SIZE (32)

extern FIFO8 keyfifo;
extern FIFO8 mousefifo;

void init_pic(void);

#endif /* INT_H */

#ifndef BOOTPACK_H
#define BOOTPACK_H

#define ADR_BOOTINFO (0x00000ff0)

typedef struct {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
} BOOTINFO;

#endif /* BOOTPACK_H */

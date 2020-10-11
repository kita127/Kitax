#ifndef BOOTPACK_H
#define BOOTPACK_H

#define ADR_BOOTINFO (0x00000ff0)

typedef struct {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
} BOOTINFO;

void putfonts8_asc(char *vram, short xsize, int x, int y, char color, char s[]);

#endif /* BOOTPACK_H */

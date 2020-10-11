#ifndef GRAPHIC_H
#define GRAPHIC_H

#define COL8_000000 (0)  /*  0:黒 */
#define COL8_FF0000 (1)  /*  1:明るい赤 */
#define COL8_00FF00 (2)  /*  2:明るい緑 */
#define COL8_FFFF00 (3)  /*  3:明るい黄色 */
#define COL8_0000FF (4)  /*  4:明るい青 */
#define COL8_FF00FF (5)  /*  5:明るい紫 */
#define COL8_00FFFF (6)  /*  6:明るい水色 */
#define COL8_FFFFFF (7)  /*  7:白 */
#define COL8_C6C6C6 (8)  /*  8:明るい灰色 */
#define COL8_840000 (9)  /*  9:暗い赤 */
#define COL8_008400 (10) /* 10:暗い緑 */
#define COL8_848400 (11) /* 11:暗い黄色 */
#define COL8_000084 (12) /* 12:暗い青 */
#define COL8_840084 (13) /* 13:暗い紫 */
#define COL8_008484 (14) /* 14:暗い水色 */
#define COL8_848484 (15) /* 15:暗い灰色 */

void init_palette(void);
void init_screen(char *vram, int xsize, int ysize);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char vram[], short vxsize, int pxsize, int pysize, int px,
                 int py, char buf[], int bxsize);
void boxfill8(char *vram, int xsize, unsigned char color, int x_s, int y_s,
              int x_e, int y_e);

#endif /* GRAPHIC_H */

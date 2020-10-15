#ifndef FIFO_H
#define FIFO_H

typedef struct {
    unsigned char *buf;
    int w;
    int r;
    int size;
    int free_num; /* 空きの数 */
    int flags;
} FIFO8;

void fifo8_init(FIFO8 *fifo, int size, unsigned char *buf);
int fifo8_put(FIFO8 *fifo, unsigned char data);
int fifo8_get(FIFO8 *fifo);
int fifo8_data_count(FIFO8 *fifo);

#endif /* FIFO_H */

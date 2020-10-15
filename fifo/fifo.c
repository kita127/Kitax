#include "./fifo.h"

#define OK            (0)
#define ERROR         (-1)
#define FLAGS_OVERRUN (0x0001)

void fifo8_init(FIFO8 *fifo, int size, unsigned char *buf) {
    fifo->size = size;
    fifo->buf = buf;
    fifo->free_num = size;
    fifo->flags = 0x00;
    fifo->w = 0;
    fifo->r = 0;
}

int fifo8_put(FIFO8 *fifo, unsigned char data) {
    if (fifo->free_num <= 0) {
        fifo->flags |= FLAGS_OVERRUN;
        return ERROR;
    }
    fifo->buf[fifo->w] = data;
    fifo->w++;
    if (fifo->w >= fifo->size) {
        fifo->w = 0;
    }
    fifo->free_num--;
    return 0;
}

int fifo8_get(FIFO8 *fifo) {
    int data;
    if (fifo->free_num >= fifo->size) {
        return ERROR;
    }
    data = fifo->buf[fifo->r];
    fifo->r++;
    if (fifo->r >= fifo->size) {
        fifo->r = 0;
    }
    fifo->free_num++;
    return data;
}

int fifo8_data_count(FIFO8 *fifo) { return fifo->size - fifo->free_num; }

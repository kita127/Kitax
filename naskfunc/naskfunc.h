#ifndef NASKFUNC_H
#define NASKFUNC_H

/* naskfunc */
void io_hlt(void);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void io_out8(int port, char data);
void io_cli(void);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

#endif  /* NASKFUNC_H */

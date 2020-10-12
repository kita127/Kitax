#ifndef NASKFUNC_H
#define NASKFUNC_H

/* naskfunc */
void io_hlt(void);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void io_out8(int port, char data);
int io_in8(int port);
void io_cli(void);
void io_sti(void);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);

#endif /* NASKFUNC_H */

# デフォルト動作
# デフォルトは先頭に書く

DEL = rm

default :
	make img

# ファイル生成規則

ipl.bin : ipl.nas Makefile
	nasm ipl.nas -o ipl.bin -l ipl.lst

kitax.sys : kitax.nas Makefile
	nasm kitax.nas -o kitax.sys -l kitax.lst

kitax.img : ipl.bin kitax.sys Makefile
	mformat -f 1440 -C -B ipl.bin -i kitax.img ::
	mcopy -i kitax.img kitax.sys ::

# コマンド

asm :
	make -r ipl.bin

img :
	make -r kitax.img

run :
	make img
	qemu-system-i386 -drive file=kitax.img,format=raw,if=floppy -boot a

clean :
	-$(DEL) ipl.bin
	-$(DEL) ipl.lst
	-$(DEL) kitax.img
	-$(DEL) kitax.sys
	-$(DEL) kitax.lst

# デフォルト動作
# デフォルトは先頭に書く

DEL = rm

default :
	make img

# ファイル生成規則

ipl10.bin : ipl10.nas Makefile
	nasm ipl10.nas -o ipl10.bin -l ipl10.lst

kitax.sys : kitax.nas Makefile
	nasm kitax.nas -o kitax.sys -l kitax.lst

kitax.img : ipl10.bin kitax.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i kitax.img ::
	mcopy -i kitax.img kitax.sys ::

# コマンド

asm :
	make -r ipl10.bin

img :
	make -r kitax.img

run :
	make img
	qemu-system-i386 -drive file=kitax.img,format=raw,if=floppy -boot a

clean :
	-$(DEL) ipl10.bin
	-$(DEL) ipl10.lst
	-$(DEL) kitax.img
	-$(DEL) kitax.sys
	-$(DEL) kitax.lst

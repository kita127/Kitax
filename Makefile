# デフォルト動作
# デフォルトは先頭に書く

default :
	make img

# ファイル生成規則

ipl.bin : ipl.nas Makefile
	nasm ipl.nas -o ipl.bin -l ipl.lst

helloos.img : ipl.bin Makefile
	mformat -f 1440 -C -B ipl.bin -i helloos.img ::

# コマンド

asm :
	make -r ipl.bin

img :
	make -r helloos.img

run :
	make img
	qemu-system-i386 -drive file=helloos.img,format=raw,if=floppy -boot a

clean :
	-rm ipl.bin
	-rm ipl.lst
	-rm helloos.img
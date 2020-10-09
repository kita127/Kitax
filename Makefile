# デフォルト動作
# デフォルトは先頭に書く

DEL = rm

default :
	make img

# ファイル生成規則

ipl10.bin : ipl10.nas Makefile
	nasm ipl10.nas -o ipl10.bin -l ipl10.lst

asmhead.bin : asmhead.nas Makefile
	nasm asmhead.nas -o asmhead.bin -l asmhead.lst

# nasm ではオブジェクトファイルとして出力する場合は -fwin32 を指定する
naskfunc.obj : naskfunc.nas Makefile
	nasm naskfunc.nas -fwin32 -o naskfunc.obj -l naskfunc.lst

hankaku.c : hankaku.txt hankaku2dat.pl Makefile
	perl hankaku2dat.pl > hankaku.c

# mac では 32bit コンパイルができないため docker で作成した ubuntu 環境でコンパイルする
# 予め環境構築した ubuntu コンテナをコンテナ名 work 作成、停止した状態にする必要がある
bootpack.hrb : bootpack.c hankaku.c naskfunc.obj Makefile
	docker start work
	docker cp . work:/root/build
	docker exec -w /root/build work gcc -march=i486 -m32 -nostdlib -fno-pic -T os.ld -o bootpack.hrb bootpack.c hankaku.c mystdio.c \
		./dsctbl/dsctbl.c ./graphic/graphic.c naskfunc.obj
	docker cp work:/root/build/bootpack.hrb bootpack.hrb
	docker exec -w /root work sh -c "rm -r ./build"
	docker stop work

kitax.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > kitax.sys

kitax.img : ipl10.bin kitax.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i kitax.img ::
	mcopy -i kitax.img kitax.sys ::

# コマンド

asm :
	make -r ipl10.bin

img :
	make -r kitax.img

start :
	docker create -it -w /root --name work ubuntu_for_xcomp

stop :
	docker rm work

run :
	make img
	qemu-system-x86_64 -drive file=kitax.img,format=raw -usb

clean :
	-$(DEL) ipl10.bin
	-$(DEL) ipl10.lst
	-$(DEL) asmhead.bin
	-$(DEL) asmhead.lst
	-$(DEL) naskfunc.obj
	-$(DEL) naskfunc.lst
	-$(DEL) hankaku.c
	-$(DEL) bootpack.hrb
	-$(DEL) kitax.sys
	-$(DEL) kitax.img

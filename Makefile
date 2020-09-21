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

# mac では 32bit コンパイルができないため docker で作成した ubuntu 環境でコンパイルする
# 予め環境構築した ubuntu コンテナをコンテナ名 work 作成、停止した状態にする必要がある
bootpack.hrb : bootpack.c naskfunc.obj Makefile
	docker start work
	docker cp bootpack.c work:/root
	docker cp naskfunc.obj work:/root
	docker exec -w /root work gcc -march=i486 -m32 -nostdlib -fno-pic -T os.ld -o bootpack.hrb bootpack.c naskfunc.obj
	docker cp work:/root/bootpack.hrb bootpack.hrb
	docker exec -w /root work sh -c "ls | grep -v "os.ld" | xargs rm"
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

docker_run :
	docker create -it -w /root --name work ubuntu_for_xcomp

docker_remove :
	docker rm work

run :
	make img
	qemu-system-i386 -drive file=kitax.img,format=raw,if=floppy -boot a

clean :
	-$(DEL) ipl10.bin
	-$(DEL) ipl10.lst
	-$(DEL) asmhead.bin
	-$(DEL) asmhead.lst
	-$(DEL) naskfunc.obj
	-$(DEL) naskfunc.lst
	-$(DEL) bootpack.hrb
	-$(DEL) kitax.sys
	-$(DEL) kitax.img

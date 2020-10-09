# デフォルト動作
# デフォルトは先頭に書く

TARGET = ./bin/kitax.img
OBJDIR = ./obj
DEL = rm

default :
	make img

# ファイル生成規則

obj : Makefile
	mkdir ./obj

bin : Makefile
	mkdir ./bin

$(OBJDIR)/ipl10.bin : ipl10.nas Makefile
	nasm ipl10.nas -o $(OBJDIR)/ipl10.bin -l $(OBJDIR)/ipl10.lst

$(OBJDIR)/asmhead.bin : asmhead.nas Makefile
	nasm asmhead.nas -o $(OBJDIR)/asmhead.bin -l $(OBJDIR)/asmhead.lst

# nasm ではオブジェクトファイルとして出力する場合は -fwin32 を指定する
$(OBJDIR)/naskfunc.obj : naskfunc.nas Makefile
	nasm naskfunc.nas -fwin32 -o $(OBJDIR)/naskfunc.obj -l $(OBJDIR)/naskfunc.lst

hankaku.c : hankaku.txt hankaku2dat.pl Makefile
	perl hankaku2dat.pl > hankaku.c

# mac では 32bit コンパイルができないため docker で作成した ubuntu 環境でコンパイルする
# 予め環境構築した ubuntu コンテナをコンテナ名 work 作成、停止した状態にする必要がある
$(OBJDIR)/bootpack.hrb : bootpack.c hankaku.c $(OBJDIR)/naskfunc.obj Makefile
	docker start work
	docker cp . work:/root/build
	docker exec -w /root/build work gcc -march=i486 -m32 -nostdlib -fno-pic -T os.ld -o bootpack.hrb bootpack.c hankaku.c mystdio.c \
		./dsctbl/dsctbl.c ./graphic/graphic.c $(OBJDIR)/naskfunc.obj
	docker cp work:/root/build/bootpack.hrb $(OBJDIR)/bootpack.hrb
	docker exec -w /root work sh -c "rm -r ./build"
	docker stop work

$(OBJDIR)/kitax.sys : $(OBJDIR)/asmhead.bin $(OBJDIR)/bootpack.hrb Makefile
	cat $(OBJDIR)/asmhead.bin $(OBJDIR)/bootpack.hrb > $(OBJDIR)/kitax.sys

$(TARGET) : obj bin $(OBJDIR)/ipl10.bin $(OBJDIR)/kitax.sys Makefile
	mformat -f 1440 -C -B $(OBJDIR)/ipl10.bin -i $(TARGET) ::
	mcopy -i $(TARGET) $(OBJDIR)/kitax.sys ::

# コマンド

asm :
	make -r $(OBJDIR)/ipl10.bin

img :
	make -r $(TARGET)

start :
	docker create -it -w /root --name work ubuntu_for_xcomp

stop :
	docker rm work

run :
	make img
	qemu-system-x86_64 -drive file=$(TARGET),format=raw -usb

clean :
	-$(DEL) -r obj
	-$(DEL) -r bin
	-$(DEL) hankaku.c

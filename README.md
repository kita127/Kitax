# Kitax
自作 OS. 実態は「30日でできる！OS入門」

## OS 起動方法

    $ ./run.sh

## 開発ツール

### qemu

PC エミュレータ

バージョン

    $ qemu-system-i386 --version
    QEMU emulator version 5.1.0
    Copyright (c) 2003-2020 Fabrice Bellard and the QEMU Project developers

img ファイル(OS が書き込まれたファイル) を PC で起動するエミュレートコマンド

    $ qemu-system-i386 -drive file=helloos.img,format=raw,if=floppy -boot a

マウスが消えた場合は ctrl+option+g でマウスをリリースできる

### NASM

アセンブラ言語

    $ nasm -v
    NASM version 2.15.04 compiled on Aug 22 2020

アセンブル

    $ nasm helloos.nas -o helloos.img

## 覚書

### ブートセクタ

ディスクの最初のセクタ(512byte)がブートセクタとなる.
PC は始めにブートセクタを読み込み最後の 2byte が 0x55、0xAA かを確認する.
0x55、0xAA でない場合は有効なプログラムが書かれていないと判断しエラーとなる.

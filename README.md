# Kitax
自作 OS. 実態は「30日でできる！OS入門」

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

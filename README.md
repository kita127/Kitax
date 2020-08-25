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

リストファイルを出力してアセンブル

    $ nasm ipl.nas -o ipl.bin -l ipl.lst

* `$`
    * 現在番地を表す
* `$$`
    * ORG の番地を表す

#### レジスタ

* 汎用？レジスタ
  * AX
    * アキュムレータ
    * 累積演算機という意味
  * CX
    * カウンタ
    * 数を数えるのに良い
  * DX
    * データ
  * BX
    * ベース
  * SP
    * スタックポインタ
  * BP
    * ベースポインタ
  * SI
    * ソースインデックス
  * DI
    * デスティネーションインデックス
* セグメントレジスタ
  * ES
    * エクストラセグメント
  * CS
    * コードセグメント
  * SS
    * スタックセグメント
  * DS
    * データセグメント
  * FS
    * 本名なし
  * GS
    * 本名なし

### Hex Fiend

バイナリエディタ

### mformat

ディスクイメージ作成ツール

install

    $ brew install mtools

実行

ディスクの先頭に ipl.bin を配置し 1440KB のイメージファイル
helloos.img を作成する

    $ mformat -f 1440 -C -B ipl.bin -i helloos.img ::


## 覚書

### ブートセクタ

ディスクの最初のセクタ(512byte)がブートセクタとなる.
PC は始めにブートセクタを読み込み最後の 2byte が 0x55、0xAA かを確認する.
0x55、0xAA でない場合は有効なプログラムが書かれていないと判断しエラーとなる.

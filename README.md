# Kitax
自作 OS. 実態は「30日でできる！OS入門」

## OS 起動方法

    $ ./run.sh

## 開発ツール

Mac 向けの使用ツールについては以下の記事を参考

[https://qiita.com/noanoa07/items/8828c37c2e286522c7ee]()

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

win32-COFF 形式のオブジェクトファイルとして出力する

    $ nasm naskfunc.nas -fwin32 -o naskfunc.obj

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

本家の `edimg.exe` の代わり

install

    $ brew install mtools

実行

ディスクの先頭に ipl.bin を配置し 1440KB のイメージファイル
helloos.img を作成する

    $ mformat -f 1440 -C -B ipl.bin -i helloos.img ::

作成したイメージファイルに kitax.sys を保存して再度イメージファイルを作成

    $ mcopy -i kitax.img kitax.sys ::

### os.ld

OS 用リンカスクリプト

入手元は以下のサイト。情報ありがとうございます

[https://vanya.jp.net/os/haribote.html](『30日でできる！OS自作入門』のメモ)

haribote OS の実行形式にビルドするためのリンカスクリプトとなっている

### C コンパイル

調査した感じ catalina では 32bit 向けクロスコンパイルに必要なコンパイラ及びツール類を揃えることが難しい
そのため docker で ubuntu 環境のコンテナを作成しそこで 32bit コンパイルする対応とした

1. docker で ubuntu のリポジトリを入手する
    * `docker pull ubunntu:18.04`
1. 作成した ubuntu コンテナに gcc をインストールする
1. ubuntu コンテナのホームディレクトリに hrb.ld を置く
1. ここまでの環境の ubuntu コンテナから image を作成する
    * イメージ名 `ubuntu_for_xcomp` で作成
1. 一旦全てのコンテナを停止、削除する
1. `ubuntu_for_xcomp` イメージからコンテナ名 `work` を作成する
1. `make bootpack.hrb` を実行する
    1. make bootpack.hrb の内訳は以下
    1. work コンテナを start
    1. コンテナにコンパイル対象の `bootpack.c` を渡す
    1. `docker exec` コマンドで `bootpack.c` をコンパイル
    1. コンパイル結果をホスト環境に渡す
    1. work コンテナを stop

GCC のコマンドは以下を参考にさせていただきました。ありがとうございます

[https://qiita.com/noanoa07/items/8828c37c2e286522c7ee](『30日でできる！OS自作入門』を macOS Catalina で実行する)

	$ gcc -march=i486 -m32 -nostdlib -fno-pic -T hrb.ld -o bootpack.hrb bootpack.c

bootpack が位置独立実行形式(position independent cod)のためのシンボル `_GLOBAL_OFFSET_TABLE_` を
参照しようとするが不要なためオプション `-fno-pic` を追加した

## 覚書

### ブートセクタ

ディスクの最初のセクタ(512byte)がブートセクタとなる.
PC は始めにブートセクタを読み込み最後の 2byte が 0x55、0xAA かを確認する.
0x55、0xAA でない場合は有効なプログラムが書かれていないと判断しエラーとなる.

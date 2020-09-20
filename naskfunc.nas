; naskfunc
; TAB=4

[BITS 32]			; 32ビットモード用の機械語を作らせる

; オブジェクトファイルのための情報

	GLOBAL	io_hlt, write_mem8		; このプログラムに含まれる関数名

; 以下は実際の関数

[SECTION .text]			; オブジェクトファイルではこれを書いてからプログラムを書く

io_hlt:		; void io_hlt(void);
	HLT
	RET

write_mem8:		; void write_mem8(int addr, int data);
	MOV		ECX,[ESP+4]
	MOV		AL,[ESP+8]
	MOV		[ECX],AL
	RET

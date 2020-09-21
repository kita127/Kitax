; naskfunc
; TAB=4

[BITS 32]			; 32ビットモード用の機械語を作らせる

; オブジェクトファイルのための情報

	GLOBAL	io_hlt		; このプログラムに含まれる関数名
	GLOBAL	io_cli
	GLOBAL	io_out8
	GLOBAL	io_load_eflags
	GLOBAL	io_store_eflags

; 以下は実際の関数

[SECTION .text]			; オブジェクトファイルではこれを書いてからプログラムを書く

io_hlt:		; void io_hlt(void);
	HLT
	RET

io_cli:		; void io_cli(void);
	CLI
	RET

io_out8:	; void io_out8(int port, char data);
	MOV		EDX,[ESP+4]		; port
	MOV		AL,[ESP+8]		; data
	OUT		DX,AL
	RET

io_load_eflags:		; int io_load_eflags(void);
	PUSHFD			; PUSH FLAG DWORD
	POP		EAX
	RET

io_store_eflags:		; void io_store_eflags(int eflags);
	MOV		EAX,[ESP+4]
	PUSH	EAX
	POPFD				; POP FLAG DWORD
	RET

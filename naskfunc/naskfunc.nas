; naskfunc
; TAB=4

[BITS 32]			; 32ビットモード用の機械語を作らせる

; オブジェクトファイルのための情報

	GLOBAL	io_hlt		; このプログラムに含まれる関数名
	GLOBAL	io_cli
	GLOBAL	io_sti
	GLOBAL	io_stihlt
	GLOBAL	io_in8
	GLOBAL	io_in16
	GLOBAL	io_in32
	GLOBAL	io_out8
	GLOBAL	io_out16
	GLOBAL	io_out32
	GLOBAL	io_load_eflags
	GLOBAL	io_store_eflags
	GLOBAL	load_gdtr
	GLOBAL	load_idtr
	GLOBAL	asm_inthandler21
	GLOBAL	asm_inthandler27
	GLOBAL	asm_inthandler2c

	EXTERN	inthandler21
	EXTERN	inthandler27
	EXTERN	inthandler2c

; 以下は実際の関数

[SECTION .text]			; オブジェクトファイルではこれを書いてからプログラムを書く

io_hlt:		; void io_hlt(void);
		HLT
		RET

io_cli:		; void io_cli(void);
		CLI
		RET

io_sti:		; void io_sti(void);
		STI
		RET

io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

io_in8:		; char io_in8(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AL,DX
		RET

io_in16:	; short io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

io_out8:	; void io_out8(int port, char data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

io_out16:	; void io_out16(int port, short data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
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

load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET


; * 割り込み完了後は IRETD 命令を実行する必要がある
; * 割り込み中に各レジスタが書き変わる可能性があるため
;   各レジスタをスタックに push し割り込み処理完了後値を復帰する
; * PUSHAD/POPAD 命令は EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI レジスタをまとめて push するのと同様
; * C言語では SS, DS, ES は同じ値でないといけない
asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

BITS 16
;org 0a100h

extern RowPos;
extern input; 全局变量
extern newline
extern century
extern year
extern month
extern day
extern hour
extern minute
extern second

extern mycmain;全局函数
extern printDate
extern printTime


global _start
_start:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	
	push cs
	call mycmain
	jmp $
	
	
	
;运行文件
;参数 文件所在扇区号
global FuncRunP
FuncRunP:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov bp,sp
	mov bx,0c100h
	mov ah,2
	mov al,1
	mov ch,0
	mov cl,byte[bp+4+16]
	mov dx,0
	int 13h
	call FuncOpenIntK
	call bx
	call FuncCloseIntK
	popa
	ret
	


;读取文件到es:bx
;参数 SecNo
global FuncReadFile
FuncReadFile:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov bp,sp
	;mov bx,FAT
	mov bx,[bp+4+16]
	mov ah,2
	mov al,1 ;一个扇区
	mov ch,0
	mov cl,byte[bp+4+4+16] ;起始扇区号
	mov dx,0
	int 13h	
	popa
	ret
	
;读取FAT到es:bx
global FuncReadFAT
FuncReadFAT:
	pusha
	mov bp,sp
	mov ax,cs
	mov ds,ax
	mov es,ax
	;mov bx,FAT
	mov bx,[bp+4+16]
	mov ah,2
	mov al,1 ;一个扇区
	mov ch,0
	mov cl,2 ;起始扇区号
	mov dh,0
	mov dl,0
	int 13h	
	popa
	ret
	


;输入字符
;输入到缓冲区 input 中
global FuncGetChar
FuncGetChar:
LabelGCCheck:
	mov ah,1
	int 16h
	jz LabelGCCheck
	mov ah,0
    int 16h
	mov byte[input],al
	mov ah,0eh
	mov bl,0 		
    int 10h
	ret
	
	
;	pusha
;	mov ah,0
;	int 16h
;	mov byte[input],al
;	popa
;	ret

;输出字符
;参数 int ch	
global FuncPrintC
FuncPrintC:
	pusha
	mov bp,sp
	mov bh,0
	mov al,byte[bp+4+16]
	;mov al,byte[input]
	mov bl,0
	mov ah,0eh
	int 10h
	popa
	ret
	
;输出字符串带换行
;参数char * buff,int len
global FuncPrintSL
FuncPrintSL:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov si,sp
	;mov	bp,buffer		 ; BP=当前串的偏移地址
	mov bp,word[si+4+16]
	;mov cx,word[BuffLen]
	mov	cx,word[si+4+4+16]  ; CX = 串长（=9）
	mov	ax,1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,byte[RowPos]	       ; 行号=0
	inc byte[RowPos]
	mov	dl,0			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
	
;	mov ax,cs
;	mov ds,ax
;	mov es,ax
	mov bp,newline ;空行
	mov cx,2
	int 10h
	;inc byte[RowPos]
	popa
    ret
	
;输出字符串不带换行
;参数char * buff,int len
global FuncPrintS
FuncPrintS:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov si,sp
	;mov	bp,buffer		 ; BP=当前串的偏移地址
	mov bp,word[si+4+16]
	;mov cx,word[BuffLen]
	mov	cx,word[si+4+4+16]  ; CX = 串长（=9）
	mov	ax,1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,byte[RowPos]	       ; 行号=0
	inc byte[RowPos]
	mov	dl,0			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
	popa
    ret
	
;输出字符串带换行指定行列
;参数char * buff,int len，int row,int col
global FuncPrintSLRC
FuncPrintSLRC:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov si,sp
	mov bp,word[si+4+16]
	mov	cx,word[si+4+4+16]  ;  串长
	mov	ax,1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,byte[si+4+4+4+16]	       ; 行号
	mov	dl,byte[si+4+4+4+4+16]			 ; 列号
	int	10h			 ; BIOS的10h功能：显示一行字符
	
	mov bp,newline ;空行
	mov cx,2
	int 10h
	popa
    ret
	
;清屏
;无参数
global FuncClear
FuncClear:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov	ax, 600h	; AH = 6,  AL = 0
	mov	bx, 700h	; 黑底白字(BL = 7)
	mov	cx, 0		; 左上角: (0, 0)
	mov	dx, 184fh	; 右下角: (24, 79)
	int	10h		; 显示中断
	mov byte[RowPos],0
	popa
	ret
	
;获取日期
;参数 century,year,month,day
global FuncGetDate
FuncGetDate:
	pusha
	mov ah,4
    int 1ah
	mov byte[century],ch
	mov byte[year],cl
	mov byte[month],dh
	mov byte[day],dl
	popa
	ret
	
;获取时间
;参数 hour,minute,second
global FuncGetTime
FuncGetTime:
	pusha
	mov ah,2
    int 1ah
    mov byte[hour],ch
    mov byte[minute],cl 
    mov byte[second],dh
	popa
	ret
	
	
global FuncSetClock
FuncSetClock:

	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	
	mov word[es:20h],FuncWindFire;SetWindFire;	设置时钟变量的偏移地址
	;mov ax,cs
	xor ax,ax
	mov word[es:22h],ax;设置段地址
	
	popa
	ret
	
	
;风火轮
WindFireChar db '/|\-'
WindFireCount dw 4
WindFireDelay1 db 2
WindFireDelay2 db 2
FuncWindFire:
	pusha
	push ds
	xor ax,ax
	mov ds,ax
	;mov es,ax
	
	dec byte[WindFireDelay1]
	jnz LabelWindFireEnd
	mov byte[WindFireDelay1],2
	mov bx,word[WindFireCount]
	cmp bx,0
	jne LabelWindFire
	mov ax,4
	mov word[WindFireCount],ax
	
	
LabelWindFire:
	mov	ax,0B800h		; 文本窗口显存起始地址
	mov	gs,ax		; GS = B800h
	mov ah,0Ah		; 0000：黑底、1010：淡绿色
	mov al,byte[WindFireChar+bx]			; AL = 显示字符值（默认值为20h=空格符）

	mov [gs:((80*24+78)*2)],ax	; 屏幕第 24 行, 第 79 列
	dec word[WindFireCount]
	dec byte[WindFireDelay2]
	jnz LabelWindFireEnd
	mov byte[WindFireDelay2],2
	
LabelWindFireEnd:
	
	push ax
	mov al,20h			; AL = EOI
	out 20h,al			; 发送EOI到主8529A
	out 0A0h,al			; 发送EOI到从8529A
	pop ax
	
	pop ds
	popa
	iret			; 从中断返回
	
TempIntKS dw 0
TempIntKO dw 0
global FuncOpenIntK
FuncOpenIntK:
	pusha
	push ds
	mov ax,cs
	mov ds,ax
	mov es,ax
	;把原有中断保存到其他地方
	push word[es:24h]
	;pop word[TempIntKO]
	pop word[es:0]
	push word[es:26h]
	pop word[es:2]
	;pop word[TempIntKS]
	
	mov word[es:24h],FuncOuch
	mov ax,0
	mov word[es:26h],ax	
	pop ds
	popa
	ret
	
global FuncCloseIntK
FuncCloseIntK:
	pusha
	push ds
	mov ax,cs
	mov ds,ax
	mov es,ax
	;push word[TempIntKO]
	push word[es:0]
	pop word[es:24h]
	;push word[TempIntKS]
	push word[es:2]
	pop word[es:26h]
	int 9h
	pop ds
	popa
	ret	
	
	
FuncOuch:
    pusha
	push ds
	
	xor ax,ax
	mov ds,ax

	mov ax,0b800h
	mov gs,ax
	mov ah,0ah
	mov al,'O'
	mov [gs:((80*1+35)*2)],ax
	mov al,'U'
	mov [gs:((80*1+36)*2)],ax
	mov al,'C'
	mov [gs:((80*1+37)*2)],ax
	mov al,'H'
	mov [gs:((80*1+38)*2)],ax
	mov al,'!'
	mov [gs:((80*1+39)*2)],ax

	pop ds
	popa
	iret
	
global FuncSetInt3X
FuncSetInt3X:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov word[es:132],FuncInt33;设置int 33
	xor ax,ax
	mov word[es:134],ax;设置段地址
	mov word[es:136],FuncInt34;设置int 34
	xor ax,ax
	mov word[es:138],ax;设置段地址
	mov word[es:140],FuncInt35;设置int 35
	xor ax,ax
	mov word[es:142],ax;设置段地址
	mov word[es:144],FuncInt36;设置int 36
	xor ax,ax
	mov word[es:146],ax;设置段地址
	popa
	ret

global FuncRunInt33
FuncRunInt33:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	int 33
	popa
	ret
	
global FuncRunInt34
FuncRunInt34:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	int 34
	popa
	ret
	
global FuncRunInt35
FuncRunInt35:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	int 35
	popa
	ret
	
global FuncRunInt36
FuncRunInt36:
	pusha
	mov ax,cs
	mov ds,ax
	mov es,ax
	int 36
	popa
	ret
	
Int33St db '17363021'
Int33SLen equ ($-Int33St)
FuncInt33:
	pusha
	push ds
	push es
	
	xor ax,ax
	mov ds,ax
	mov es,ax
	
	mov	bp,Int33St		 ; BP=当前串的偏移地址
	mov cx,Int33SLen  ; CX = 串长（=9）
	mov	ax,1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,5       ; 行号=0
	mov	dl,60			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
	mov bp,newline ;空行
	mov cx,2
	int 10h
	pop es
	pop ds
	popa
	iret
	
Int34St db 'Guan Dongyang'
Int34SLen equ ($-Int34St)
FuncInt34:
	pusha
	push ds
	push es
	
	xor ax,ax
	mov ds,ax
	mov es,ax
	
	mov	bp,Int34St		 ; BP=当前串的偏移地址
	mov cx,Int34SLen  ; CX = 串长（=9）
	mov	ax,1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx,0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh,6       ; 行号=0
	mov	dl,60			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
	mov bp,newline ;空行
	mov cx,2
	int 10h
	pop es
	pop ds
	popa
	iret

FuncInt35:
	pusha
	push ds
	push es
	xor ax,ax
	mov ds,ax
	mov es,ax
	
	push cs
	call printDate
	
	pop es
	pop ds
	popa
	iret
	
FuncInt36:
	pusha
	push ds
	push es
	xor ax,ax
	mov ds,ax
	mov es,ax
	
	push cs
	call printTime
	
	pop es
	pop ds
	popa
	iret
	

;写IO端口
;参数port 16,value 8
Global FuncIOout:	
FuncIOout:
	push bp
	mov bp,sp
	mov dx,word[bp+4+2];port
	mov al,byte[bp+4+4+2];value
	out dx,al
	nop
	nop
	mov sp,bp
	pop bp
	ret
	
;读IO端口	
;参数port 16
Global FuncIOin:	
FuncIOin:
	push bp
	mov bp,sp
	mov dx,word[bp+4+2];port
	xor ax,ax
	in al,dx
	;mov byte[input],al
	nop
	nop
	mov sp,bp
	pop bp
	ret

delay1 dw 5
delay2 dw 5
FuncDelay:
	push cx
	mov cx,word[delay1]
	push cx
LabelDelay1:
	mov cx,word[delay2]
	loop $
	pop cx
	loop LabelDelay1
	pop cx
	
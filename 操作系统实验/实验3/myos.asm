BITS 16
;org 0a100h

extern RowPos;
extern input; 全局变量
extern newline
extern mycmain

global _start
_start:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	;push cs
	call mycmain
	jmp $
	
	
	
;运行文件
;参数 文件所在扇区号
global FuncRunP
FuncRunP:
	pusha
	mov bp,sp
	mov ax,cs
	mov es,ax
	mov bx,0c100h
	mov ah,2
	mov al,1
	mov ch,0
	mov cl,byte[bp+4+16]
	mov dx,0
	int 13h
	call bx
	popa
	ret
	
;读取文件到es:bx
;参数 SecNo
global FuncReadFile
FuncReadFile:
	pusha
	mov bp,sp
	mov ax,0
	mov es,ax
	mov ds,ax
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
	mov ax,0
	mov es,ax
	mov ds,ax
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
	pusha
	mov ah,0
	int 16h
	mov byte[input],al
	popa
	ret

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
	mov ax,ds
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
	mov ax,ds
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
	
;清屏
;无参数
global FuncClear
FuncClear:
	pusha
	mov	ax, 600h	; AH = 6,  AL = 0
	mov	bx, 700h	; 黑底白字(BL = 7)
	mov	cx, 0		; 左上角: (0, 0)
	mov	dx, 184fh	; 右下角: (24, 79)
	int	10h		; 显示中断
	mov byte[RowPos],0
	popa
	ret
	
;times 2560-($-$$) db 0
	
;Data:
;	SegFAT equ 0a100h
;	OffFAT equ 0h
;	NumSecFAT equ 1
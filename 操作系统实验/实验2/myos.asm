off_a equ 0a100h
off_b equ 0a150h
off_c equ 0a200h
off_d equ 0a250h

org 7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行

Start:
	mov	ax, cs	       ; 置其他段寄存器值与CS相同
	mov	ds, ax	       ; 数据段
	mov	bp, Message		 ; BP=当前串的偏移地址
	mov	ax, ds		 ; ES:BP = 串地址
	mov	es, ax		 ; 置ES=DS
	mov	cx, MessageLength  ; CX = 串长（=9）
	mov	ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dh, [pos]		       ; 行号=0
	mov	dl, 0			 ; 列号=0
	inc byte[pos]
	inc byte[pos]
	int	10h			 ; BIOS的10h功能：显示一行字符
	jmp Input

Tip:
	mov	ax, cs
	mov	ds, ax
	mov	bp, TipMessage
	mov	ax, ds
	mov	es, ax
	mov	cx, TipLen
	mov	ax, 1301h
	mov	bx, 0007h
	mov dh, [pos]
	inc byte[pos]
	inc byte[pos]
	mov	dl, 0
	;cmp dh,24
	;jz Reflash
	int	10h

Input:
;	call Newline
	mov ax,cs
	mov es,ax
	mov ah,0 ;读键盘输入的字符
	int 16h


	mov ah,0eh;显示字符
	mov bh,0
	mov bl,0
	int 10h

	call Newline

	cmp al,'a'
	jz A
	cmp al,'b'
	jz B
	cmp al,'c'
	jz C
	cmp al,'d'
	jz D
	jmp Tip

A:
	;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
	mov ax,cs                ;段地址 ; 存放数据的内存基地址
	mov es,ax                ;设置段地址（不能直接mov es,段地址）
	mov bx,off_a  ;偏移地址; 存放数据的内存偏移地址
	mov ah,2                 ; 功能号
	mov al,1                 ;扇区数
	mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
	mov dh,0                 ;磁头号 ; 起始编号为0
	mov ch,0                 ;柱面号 ; 起始编号为0
	mov cl,2                 ;起始扇区号 ; 起始编号为1
	int 13H ;                调用读磁盘BIOS的13h功能
	call off_a
	jmp Start
	; 用户程序a.com已加载到指定内存区域中

B:
	mov ax,cs
	mov es,ax
	mov bx,off_b
	mov ah,2
	mov al,1
	mov dl,0
	mov dh,0
	mov ch,0
	mov cl,3
	int 13H ;
	call off_b
	jmp Start
	
C:
	mov ax,cs
	mov es,ax
	mov bx,off_c
	mov ah,2
	mov al,1
	mov dl,0
	mov dh,0
	mov ch,0
	mov cl,4
	int 13H ;
	call off_c
	jmp Start

D:
	mov ax,cs
	mov es,ax
	mov bx,off_d
	mov ah,2
	mov al,1
	mov dl,0
	mov dh,0
	mov ch,0
	mov cl,5
	int 13H ;
	call off_d
	jmp Start


AfterRun:
	jmp Tip

Newline:
	push ax
	push bx
	mov ah,0eh
	mov al,0ah
	mov bl,0
	int 10h
	mov ah,0eh
	mov al,0dh
	mov bl,0
	int 10h
	pop bx
	pop ax
	ret

Data:
	Message:db 'Hello,OS start successfully!Please input a,b,c,d to load program...',0ah,0dh
	MessageLength equ ($-Message)
	TipMessage:db 'Common error!Please input again...',0ah,0dh
	TipLen equ ($-TipMessage)
	pos:db 00h ;位置信息 行
	times 510-($-$$) db 0
	dw 0xaa55


	max_r equ 25
	max_c equ 80
	delay_a equ 50000
	delay_b equ 3000	
	org 0c100h
	
Init:
	xor ax,ax
	mov ax,cs
	mov ds,ax
	mov ss,ax
	mov es,ax
	
	mov ax,0b800h ;显存起始地址
	mov es,ax
	mov cx,count ;次数
	
Start:
	;循环显示count次
	push cx
	call Delay
	call Print
	pop cx
	dec cx
	jnz Start
	ret
	
Print:
	mov ax,[pos]
	mov bx,[dir]
	add al,bl
	
Up:
	cmp al,13
	jne Dn
	mov bl,1 ;往下
	
Dn:
	cmp al,max_r-1
	jl Lf ;没有碰到下墙壁之前
	mov bl,-1;碰到了就往上
	
Lf:
	add ah,bh
	cmp ah,43
	jne Rt
	mov bh,1;往右
	
Rt:
	cmp ah,max_c
	jl Next
	mov bh,-1;碰到了就往左
	
Next:
	;将上一次显示的字符串清零
	push ax
	push bx
	mov ax,0
	mov ah,[clear]
	mov si,string_cl
	call Show
	
	;显示字符串
	pop bx
	pop ax
	mov [pos],ax
	mov [dir],bx
	mov ax,0
	mov ah,[color]
	mov si,string
	sub ax,[pos]
	mov al,0
	call Show
	ret
	
Show:
	;逐个显示字符串中的字符
	push cx
	mov cx,[len];获取串长	
	loop_st:
		push ax
		mov bx,[pos];取c r
		mov ax,0
		mov al,bl;取R，并计算第几行
		mov dx,80
		mul dx;结果 高位dx,低位ax
		mov dx,0;只需取低位
		
		mov dl,bh;取C，第几列
		add ax,dx;
		sub ax,cx
		mov dx,2;算出显存地址 (R*80+C)*2
		mul dx
		mov bp,ax;将结果保存到bp中
		
		pop ax;取回字符属性
		mov al,[si];取字符
		inc si
		mov [es:bp],ax ;显示
		dec cx
		jnz loop_st
	pop cx
	ret
	
Delay:
	mov cx,delay_a
	loop1:
	mov ax,delay_b
	loop2:
	dec ax
	jnz loop2
	dec cx
	jnz loop1
	ret
	
	
Data:
	string_cl:db "***"
	string:db "TAT"
	len: dw $-string
	clear: db 00000000b ;清零
	count equ 100 ;弹射次数
	color: db 10010111b ;颜色
	dir: dw 0101h ;速度，起始 bh左右，bl上下
	pos:dw 4412h ;位置信息 ah 列 al 行

times 512-($-$$) db 0
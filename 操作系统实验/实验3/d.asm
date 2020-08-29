	org 0c100h
	
Init:
	xor ax,ax
	mov ax,cs
	mov ds,ax
	mov ss,ax
	mov es,ax
	
	mov ax,info;输出info
	mov bp,ax
	mov cx,[len_info]
	mov ax,01301h
	mov bx,10110001b
	mov dh,6
	mov dl,45
	int 10h
	ret
	
Data:
	info:db "17363021 GuanDongyang"
	len_info: dw $-info	
	
	times 512-($-$$) db 0
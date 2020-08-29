;%define	_BOOT_DEBUG_	; 做 Boot Sector 时一定将此行注释掉!将此行打开后用 nasm Boot.asm -o Boot.com 做成一个.COM文件易于调试

%ifdef	_BOOT_DEBUG_
	org  0100h			; 调试状态, 做成 .COM 文件, 可调试
%else
	org  07c00h			; Boot 状态, Bios 将把 Boot Sector 加载到 0:7C00 处并开始执行
%endif 

	;jmp short LABEL_Start		; Start to boot.
	;nop				; 这个 nop 不可少

Label_Start:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov si,Message
	call Func_Print
	
Label_LoadOS:
	mov ax,Seg_OS
	mov es,ax
	mov bx,Off_OS
	mov ah,2
	mov al,Num_Sec_OS;扇区数目
	mov ch,Cyl_OS;柱面
	mov cl,Sec_OS;扇区号
	mov dh,1;磁头
	mov dl,0;软驱
	int 13h
	jmp Off_OS
	

;输出字符串
;参数：SI 字符串地址
Func_Print:
    lodsb
    or al, al
    jz Label_PrintDone
    mov ah, 0eh
    int 10h
    jmp Func_Print
Label_PrintDone:
    ret
	
Data:
	Message db 'Loading...',0ah,0dh,0
	Off_OS equ 0a100h
	Seg_OS equ 0;4096
	Cyl_OS equ 0
	Sec_OS equ 1
	Num_Sec_OS equ 18;OS所占扇区数
	times 510-($-$$) db 0
	dw 0xaa55
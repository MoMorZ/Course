@echo off
:begin
nasm -f elf32 myos.asm -o myos.o
i686-elf-gcc -c -ffreestanding -m32 -march=i386 -mpreferred-stack-boundary=2 mycmain.c -o mycmain.o
i686-elf-ld -N myos.o mycmain.o -Ttext=0xa100 -m elf_i386 --oformat binary -o ac.bin
pause
goto begin
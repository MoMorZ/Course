.data
	arr:.space 40
	string1:.asciiz "Please input numbers:\n"
	string2:.asciiz "Answer:\n"
.text
.globl main
main:
	move $s0 $zero #计数器
	addi $s1 $zero 10 #10个数
	la $a0 string1 #字符串1的地址装入a0
	li $v0 4 #输出字符串
	syscall
	
input:
	beq $s0 $s1 sort #比较，相等结束读入
	la $s6 arr #把数组的首地址复制到$s6
	sll $t0 $s0 2 #计数器乘4保存到t0
	addu $s6 $s6 $t0 #把数组当前元素地址移动
	li $v0,5 #从键盘读入一个整数
	syscall
	sw $v0 0($s6) #把读入的数存入当前数组元素地址中
	addi $s0 $s0 1 #计数器加1
	j input

sort:
	move $s3 $zero #计数器i
	addi $s1 $zero 9 #外部循环终止条件

forout:
	slt $t3 $s3 $s1 #如果i<len-1,t3=1
	beq $t3 $zero output#是否到达外部循环终止条件，是退出
	move $s4 $zero #计数器j=0
	addi $s2 $zero 9 #内部循环终止条件
	sub $s2 $s2 $s3 #终止条件len-1-i
	sll $t0 $s3 2 #计数器i乘4保存到t0
	
forin:
	slt $t3 $s4 $s2 #如果j<len-1-i,t3=1
	beq $t3 $zero contout#是否到达内部循环终止条件，是退出
	sll $t0 $s4 2 #计数器j乘4保存到t0
	la $s6 arr #把数组的首地址复制到$s6
	addu $s6 $s6 $t0 #把数组当前元素地址移动
	lw $t1 0($s6) #把arr[j]保存到t1中
	lw $t2 4($s6) #把arr[j+1]保存到t2中
	sltu $t3 $t1 $t2 #如果arr[j]<arr[j+1]，t3=1
	beq $t3 $zero contin #arr[j]>arr[j+1] 直接进入是否下一个内层循环的判断
	sw $t2 0($s6) #arr[j]=arr[j+1]
	sw $t1 4($s6) #arr[j+1]=t1=arr[j]

	
contin: #下一个内层循环
	addi $s4 1 #j++
	j forin
	
contout: #下一个外层循环
	addi $s3 1 #i++
	j forout
	
output:
	move $s0 $zero #计数器
	addi $s1 $zero 10 #10个数
	la $a0 string2 #字符串2的地址装入a0
	li $v0 4 #输出字符串
	syscall
	
print:
	beq $s0 $s1 exit #循环条件检查
	la $s6 arr #把数组的首地址复制到$s6
	sll $t0 $s0 2 #计数器乘4保存到t0
	addu $s6 $s6 $t0 #把数组当前元素地址移动
	lw $a0 0($s6)
	li $v0,1 #输出一个整数到屏幕
	syscall
	li $a0 32 #空格
	li $v0 11 #输出空格
	syscall
	addi $s0 1 #循环变量+1
	j print
	
exit:
	li $a0 10 #换行
	li $v0 11 #输出换行
	syscall
	li $v0 10 #退出系统调用
	syscall
	
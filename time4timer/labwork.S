  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global hexasc, time2string, delay, enable_interrupt
.data
.text
 hexasc:	
	sll	$t2, $a0, 28
	srl	$t2, $t2, 28	
	slti	$t0, $t2, 10
	addi	$t1, $t2, 48
	bne	$t0, $0, return
	nop
	addi	$t1, $t1, 7
return:
	move	$v0, $t1
	jr	$ra
	nop
		
time2string: 	
		
	
	PUSH	$ra		
	la	$t3, ($a0)
	addi	$t7, $0, 0x3a		# ':'-ascii kod, storlek 1 byte -> t7	
	sb	$t7, 2($t3)		# ':'-byte sparas
	sb	$0, 5($t3)		# null-byte sparas
	

	
	andi	$t4, $a1, 0xffff

	andi	$a0, $t4, 0xf
	jal 	hexasc
	nop
	sb	$v0, 4($t3)
	
	srl	$t4, $t4, 4
	andi	$a0, $t4, 0xf
	jal 	hexasc
	nop
	sb	$v0, 3($t3)
	
	srl	$t4, $t4, 4
	andi	$a0, $t4, 0xf
	jal 	hexasc
	nop
	sb	$v0, 1($t3)
	
	srl	$t4, $t4, 4
	andi	$a0, $t4, 0xf
	jal 	hexasc
	nop
	sb	$v0, ($t3)
	
	la	$a0, ($t3)
	
	
	POP	$ra
	nop
	jr	$ra
	nop
 	

delay:
	PUSH	$ra
	nop
	addi $t1,$0,0

		
for:
	addi $t1,$t1,1
	bne $t1,7500,for
	nop
	
while:
	addi $a0,$a0,-1
	bnez $a0,delay
	nop
	
	POP	$ra
	
	jr	$ra
	nop

enable_interrupt:
	ei
	jr $ra
	nop
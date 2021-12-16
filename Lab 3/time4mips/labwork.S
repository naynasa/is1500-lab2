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
	
	#s registers are callee saved save the ones we want to use
	PUSH	$s0 #callee saved and used for storing a0
	PUSH	$s1 #callee saved and used for storing a1
	PUSH	$ra #to be able to return since we call hexasc
	
	#used but not saved registers: a0,a1,v0

	
	addi	$s0,$a0,0 #save a0 in s0 since hexasc will write over a0
	addi	$s1,$a1,0 #save a1 in s1 since hexasc could write over a1
	

	#v0 stores the current byte we are working on before writing it to memory
	
	
	#minute * 10
	#andi	$a0,$s1,0x0000f000 #a0 = 4th nibble of a1
	srl	$a0,$s1,12 #make the 4rth nibble the 4 LS bits in a0 - [XXXX...YYYY XXXX XXXX XXXX] -> [00000...XXXX...YYYY]
	jal	hexasc  #v0 -  8 LS bits ascii representation of $a0
	nop #branch delay slot
	sb	$v0,0($s0)#write v0 to memory #v0 contains the correct byte of 4th nibble of a1
	#s0 = 0xfba22184
	#[AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD EEEEEEEE 00000000]
	#^s0
	#minute * 1
	#andi	$a0,$s1,0x00000f00 #a0 = 3rd nibble of a1
	#s0=0xABCD
	#a0=0x00AB
	#hexasc(0xAB) <=> hexasc(0xB)
	srl	$a0,$s1,8 #make the 3rd nibble the 4 LS bits in a0
	jal	hexasc #v0 -  8 LSB ascii representation of $a0
	nop #branch delay slot
	sb	$v0,1($s0)#write v0 to memory #v0 contains the correct byte of 3rd nibble of a1
	
	#colon
	li	$v0,0x3A
	sb	$v0,2($s0)#write v0 to memory #v0 contains the correct byte of the colon
	
	#second * 10
	#andi	$a0,$s1,0x000000ff #a0 = 2nd nibble of a1
	srl	$a0,$s1,4 #make the 2nd nibble the 4 LS bits in a0 #this handles the 5
	jal	hexasc #v0 -  8 LSB ascii representation of $a0
	nop #branch delay slot
	sb	$v0,3($s0)#write v0 to memory #v0 contains the correct byte of 3rd nibble of a1
	
	#second * 1
	andi	$a0,$s1,0x0000000f #a0 = 1st nibble of a1, sista 4 bitarna 
	
	li	$t0,2 
	bne    	$a0,$t0, second_if # $a0 != 2
	nop #branch delay slot
	j	second_else #else
	nop #branch delay slot
	second_if:

		jal	hexasc #v0 -  8 LSbits ascii representation of $a0
		nop #branch delay slot
		sb	$v0,4($s0) #l�gger in v�rdet i index 4 (= 5te elementet)
		
		li	$v0,0
		sb	$v0,5($s0)#write v0 to memory #v0 contains the correct byte of the colon
		
		j 	second_continue
		
		nop

	second_else:
 
		li $a0, 0x004F5754 #TWO
		sw $a0, 4($s0) #l�gger in v�rdet i s0, offset 4, b�rja l�gga in p� slutet
		#[AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD TTTTTTTT WWWWWWWW OOOOOOO EEEEEEEE]
		#^s0
		#li	$v0,0x0000
		#sb	$v0,8($s0)# 8($s0), d�r den 8onde biten slutar. 0 byten signalerar att det �r slut p� text str�ngen 
		#[AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD NNNNNNNN IIIIIIII NNNNNNNN EEEEEEEE 00000000]
		#^s0
		
	second_continue:	
	
	#it's possible these shouldn't be here but i think they should

	POP	$ra #to be able to return since we call hexasc
	POP	$s1 #callee saved and used for storing a1
	POP	$s0 #callee saved and used for storing a0
	
	
	jr $ra
	nop #branch delay slot
		
		
		
	

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
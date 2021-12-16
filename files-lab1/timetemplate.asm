# timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,-1
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop 
	# go back and do it all again
	j	main
	nop
	
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #returns in the format 0000.0[ZZZZZZZZ]
 hexasc:   
	andi	$a0,$a0,0x0000000f #XXXX...X[YYYY] AND 0000..0[1111] = 0000.0[YYYY], 0x0000000f =15
	li	$t0,9
	ble    	$a0,$t0,hexasc_if#$a0<10
	j	hexasc_else #else
	nop #branch delay slot
	hexasc_if:
		#we know that $a0<10
		addi	$a0,$a0,48 #0 <=> ASCII 48, 1 <=> ASCII 49 etc.
		j hexasc_contine
		nop #branch delay slot
	hexasc_else:
		addi	$a0,$a0,55 #65-10 since A <=> ASCII 65, A in hex is 10 in base 10
	hexasc_contine:	
	li	$v0,0
	add	$v0,$a0,$v0
	jr	$ra
	nop #branch delay slot
#two loops representing a delay of $a0 ms
#outer loop takes 1 ms per iteration and inner loop is to make the outer loop take 1 ms
.global
delay:
	ble	$a0,$zero,delay_while_end #a0 != 0 goto while
	nop #branch delay slot
	j	delay_while #if a==0 jump to the end
	nop #branch delay slot
	delay_while:
		addi	$a0,$a0,-1 #if $a0 is -1 from start it becomes more and more neg. until the program crashes / undefined behavior
		
		li	$t0,0 #i=0
		li	$t1,60000# number to check i against - number of iterations that fit in around 1ms
		addi	$t1,$t1,-1 # i<4711 <=> i<=4710 
		
		ble	$t0,$t1,delay_for #initial check if we should go into the loop
		nop	#in case of branch delay slot
		j	delay_for_end
		nop #branch delay slot
		delay_for:
			addi	$t0,$t0,1 #i = i + 1
			#body
			nop # <=> /* Do nothing. */
			ble	$t0,$t1,delay_for #check if we should continue looping each iteration
			nop #branch delay slot
		delay_for_end:	
			bne	$a0,$zero,delay_while
			nop #branch delay slot

	delay_while_end:
	jr	$ra
	nop #branch delay slot

#		void delay( int ms ) /* Wait a number of milliseconds, specified by the parameter value. */
#{
#int i;
#while( ms > 0 )
#{
#ms = ms – 1;
#/* Executing the following for loop should take 1 ms */
#for( int i = 0; i < 4711; i = i + 1 ) /* The constant 4711 must be easy to change! */
#{
#/* Do nothing. */
#}
#}
#}


#$a0 the adress to where we write the ascii time - value stored in $a0 format: mm:ssNULLBYTE
#$a0 needs 6*8 bits of space to store the string (ascii char. are 8 bit)
#$a1 time of the format mmss in a 32 bit value representation - 4 bit per value - only 16 least sig. bit used
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
		sb	$v0,4($s0) #lägger in värdet i index 4 (= 5te elementet)
		
		li	$v0,0
		sb	$v0,5($s0)#write v0 to memory #v0 contains the correct byte of the colon
		
		j 	second_continue
		
		nop

	second_else:
 
		li $a0, 0x004F5754 #TWO
		sw $a0, 4($s0) #lägger in värdet i s0, offset 4, börja lägga in på slutet
		#[AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD TTTTTTTT WWWWWWWW OOOOOOO EEEEEEEE]
		#^s0
		#li	$v0,0x0000
		#sb	$v0,8($s0)# 8($s0), där den 8onde biten slutar. 0 byten signalerar att det är slut på text strängen 
		#[AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD NNNNNNNN IIIIIIII NNNNNNNN EEEEEEEE 00000000]
		#^s0
		
	second_continue:	
	
	#it's possible these shouldn't be here but i think they should

	POP	$ra #to be able to return since we call hexasc
	POP	$s1 #callee saved and used for storing a1
	POP	$s0 #callee saved and used for storing a0
	
	
	jr $ra
	nop #branch delay slot
		
		
		

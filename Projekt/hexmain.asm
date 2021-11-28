  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,17		# change this to test different values
	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #
#hexasc(15) => F
#hexasc(0) => etc.
#extracts the least significant bits

#parameter $a0 - 4 least significant bits are turned into ascii hex representation all others are ignored
#return $v0 - 7 significant bits ascii representation of $a0
	.text
hexasc:
	andi	$a0,$a0,15#XXXX...X[YYYY] AND 0000..0[1111] = 0000.0[YYYY]
	li	$t0,9
	ble    	$a0,$t0,hexasc_if#$a0<10
	j	hexasc_else #else
	hexasc_if:
		#we know that $a0<10
		addi	$a0,$a0,48 #0 <=> ASCII 48, 1 <=> ASCII 49 etc.
		j hexasc_contine
	hexasc_else:
		addi	$a0,$a0,55 #65-10 since A <=> ASCII 65, A in hex is 10 in base 10
	hexasc_contine:	
	li	$v0,0
	add	$v0,$a0,$v0
	jr	$ra


	
#for(int i=0; i<3;i++{
#bytes #4 sista bitarna
#heasc(bytes)
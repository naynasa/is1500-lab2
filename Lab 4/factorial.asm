# we can use $0, $at, $v0, $v1, $a0, $a1, $a2, and $a3
.data

.text


addi $a0,$0,5 #$a0 = n here 5
addi $v0,$0,1 #$v0 = sum = 1
addi $a1,$0,1 #$a1 holds our constant to compare against

loop_start: beq $a0,$a1,loop_end #if n== 1 go to loop_end <=> while(n>1)
# nop not needed since we dont use a branch delay slot

#multiplication
add $a2,$a0,$0 #b = n, $a2 = b
add $v1,$v0,$0 #$v1 = sum before inner loop
inner_loop_start: beq $a2,$a1,inner_loop_end #if b== 1 go to inner_loop_end <=> while(b > 1)

add $v0,$v0,$v1
addi $a2,$a2,-1
beq $0,$0,inner_loop_start
inner_loop_end:

addi $a0,$a0,-1
beq $0,$0,loop_start

loop_end:


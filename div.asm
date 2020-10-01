add $t0, $zero, $imm, 4095
sll $t0, $t0, $imm, 4
or $t0, $t0, $imm, 15
add $sp, $zero, $imm, 2048	# set $sp = 2048
lw $a0, $zero, $imm, 1024	# get x from address 1024
and $a0, $a0, $t0
lw $a1, $zero, $imm, 1025	# get y from address 1025
jal $imm, $zero, $zero, div
sw $v0, $zero, $imm, 1026
sra $v0, $v0, $imm, 16
sw $v0, $zero, $imm, 1027
halt $zero, $zero, $zero	# halt
div:
add $sp, $sp, $imm, -5		# adjust stack for 5 items
sw $s1, $sp, $imm, 4
sw $s0, $sp, $imm, 3		
sw $ra, $sp, $imm, 2		# save return address
sw $a1, $sp, $imm, 1
sw $a0, $sp, $imm, 0		# save argument

add $s0, $zero, $zero		# the integer part of the division
add $s1, $a0, $zero		# the remainder
iteration:
ble $imm, $a1, $s1, loop

exit:
add $v0, $zero, $s1
sll $v0, $v0, $imm, 16
add $v0, $v0, $s0
lw $a0, $sp, $imm, 0		# restore $a0
lw $a1, $sp, $imm, 1		# restore $a1
lw $ra, $sp, $imm, 2		# restore $ra
lw $s0, $sp, $imm, 3		# restore $s0
lw $s1, $sp, $imm, 4		# restore $s1
add $sp, $sp, $imm, 5		# pop 5 items from stack
beq $ra, $zero, $zero		# and return
loop:
sub $s1, $s1, $a1
add $s0, $s0, $imm, 1
beq $imm, $zero, $zero, iteration
.word 1025 22
.word 1024 45








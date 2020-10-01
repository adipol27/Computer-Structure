
.word 1024 1 # load line 0 to memory

lw $a0, $zero, $imm, 1023	# load the input argument to $a0
add $s0, $imm, $zero, 1	# $s0 = Current Line
add $s1, $imm, $zero, 0	# follows the offset
bgt $imm, $a0, $s1, newLine # if the argument is 0, we finished.

stop:
halt $zero, $zero, $zero	

newLine:	
add $s0, $s0, $imm, 1	 # increase the line number
lw $t0, $s0, $imm, 1023	
sw $t0, $s0, $imm, 1024  # move the value of column $s0 to the next column
sub $s1, $s0, $imm, 1 # $s1 is decreased each iteration of the loop

loop:		
lw $t0, $s1, $imm, 1023		# move the current column forward
lw $t1, $s1, $imm, 1024		
add $t2, $t0, $t1		# calculate the new column value
sw $t2, $s1, $imm, 1024	# and store it
sub $s1, $s1, $imm, 1		
bgt $imm, $s1, $zero, loop # There are more columns in the current line
ble $imm, $s0, $a0, newLine	# Start new line
beq $imm, $zero, $zero, stop 	

.word 1023 4	# input !
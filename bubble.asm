.word 1024 246
.word 1025 352
.word 1026 27
.word 1027 34
.word 1029 -2
.word 1031 432
.word 1032 -22
.word 1033 0x34
.word 1030 543
.word 1037 34
.word 1038 56
.word 1039 44
add $a1, $imm, $zero, 1024			# $a1 contains the index of the first number in the array
add	$s1, $zero, $zero				# $s1 is a counter
.word 1035 176
add $s2, $zero, $zero				# $s2 - offset in the array
.word 1028 -87
add $a0, $imm, $zero, 16			# $a0 contains the number of members of the array 
.word 1028 -87
doublefor:								# Bubbluesort loop
add $t3, $a1, $s2					# $t3 - the pair we compare 
lw $t0, $t3, $imm, 0				# $t0 contains the first number in the pair
lw $t1, $t3, $imm, 1				# $t1 contains the second number in the pair
sub $t2, $t1, $t0					# $t2 = the second number - the first number
.word 1034 87
ble $imm, $zero, $t2, end			# 0<=$t2 => don't swap!
sw $t1, $s2, $imm, 1024				# SWAP
sw $t0, $s2, $imm, 1025

end:								# Exit loop
add $s2, $s2, $imm, 1				# increase the offset on the array
sub	$t0, $a0, $imm, 1				
bne $imm, $s2, $t0, doublefor		# We check if the offset equals ($a0-1)	!!!!!!
add	$s1, $s1, $imm, 1				# increase the counter
add $s2, $zero, $zero				# now the offset is 0 once again....	
bne $imm, $s1, $a0, doublefor		# the counter reached the number of members of the array?
halt $zero, $zero, $zero
# End of BubbleSort



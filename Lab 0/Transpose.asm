.data
# This is the start of the original array.
Original: 	.word 200, 270, 250, 100
		  	.word 205, 230, 105, 235
			.word 190, 95, 90, 205
			.word 80, 205, 110, 215
# The next statement allocates room for the other array.
# The array takes up 4*16=64 bytes.
#
Second: 	.space 64

.align 2

.globl main
.text

main: 
	# Program runs through each row sequentially in Original array
	# transposing the contents into Second's columns

	# t1 - Original offset
	# t2 - Second offset
	# t3 - Array value
	# t4 - Second column offset
	# t5 - Exit condition (64)

	# init register values
	li 		$t1, 0
	li 		$t2, 0
	li 		$t3, 0
	li 		$t4, 0
	li 		$t5, 64

NextRow:
	# begin at Second column designated by Second offset
	move 	$t2, $t4
CurrentRow:
	# Load row value from Original into corresponsing column element in Second
	lw		$t3, Original($t1)
	sw		$t3, Second($t2)

	# Move to next row element for Original and next column element for Second
	addi	$t1, $t1, 4
	addi	$t2, $t2, 16

	# Loop to CurrentRow if Second column not full
	sub		$t3, $t2, $t5
	bltz 	$t3, CurrentRow

	# Otherwise, increment Second row offset
	addi	$t4, $t4, 4

	# Loop to NextRow if row finished transposing and elements still remain to transpose
	sub		$t3, $t1, $t5
	bltz 	$t3, NextRow


Exit: 
	li $v0, 10 #terminate program
	syscall

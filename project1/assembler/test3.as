	lw	0	2	a
	lw	0	3	b
	lw	0	7	sumAdr
start	jalr	7	6		do add function
	halt
sum	add	2	3	1
	jalr	6	7		go back
sumAdr	.fill	sum
a	.fill	10
b	.fill	20

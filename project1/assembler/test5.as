	lw	0	8	zero	i=0
	lw	0	1	n
	lw	0	2	one
loop	beq	8	1	done
loop	add	8	2	8
	beq	0	0	loop
done	halt
zero	.fill	0
n	.fill	4
one	.fill	1
array	.fill	1
	.fill	2
	.fill	3
	.fill	4

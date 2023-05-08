	noop				boj.kr 10807
	add	0	0	1	reg1 = answer
	add	0	0	2	reg2: i=0
	lw	0	3	pos1	reg3 = 1
	lw	0	4	N	reg4 = N
	lw	0	5	V	reg5 = V
loop	beq	2	4	done	if i == N, break
	lw	2	6	array	get array[i]
	beq	6	5	count	array[i] == V
	beq	0	0	after 	array[i] != V
count	add	1	3	1	answer++
after	add	2	3	2	i++
	beq	0	0	-7	goto for loop
done	halt
pos1	.fill	1
N	.fill	11
V	.fill	2
array	.fill	1
	.fill	4
	.fill	1
	.fill	2
	.fill	4
	.fill	2
	.fill	4
	.fill	2
	.fill	3
	.fill	4
	.fill	4

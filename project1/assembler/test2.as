	noop				bubble sort!
	lw	0	1	N	reg1 = N
	lw	0	2	pos1	reg2 = 1
	add	0	0	3	reg3: i=0
oloop	beq	3	1	done	
	nor	3	3	5	reg5 = -i-1
	add	5	3	5	reg5 = -i
	add	1	5	5	reg5 = N-i
	add	0	0	4	reg4: j=0
iloopb	beq	4	5	iloopa  if j == N-i, break for-loop
	lw	4	6	array	reg6 = array[j]
	add	4	2	7	reg7 = j+1
	lw	7	7	array	reg7 = array[j+1]
	sw	0	7	temp	mem[temp] = array[j+1]
	nor	0	7	7	reg7 = -array[j+1]-1
	add	7	2	7	reg7 = -array[j+1]
	add	6	7	7	reg7 = array[j]-array[j+1]  test랑 nor했을때 0이면 음수
	lw	0	6	test	reg6 = test number
	nor	7	6	7	reg7 = array[j]-array[j+1] nor 0x7fffffff
	beq	7	0	outif	not in if {} !!!!!
	lw	4	6	array	reg6 = array[j]
	add	4	2	2	reg2 = j+1
	lw	2	7	array	reg7 = array[j+1]
	sw	4	7	array	mem[array+j] = array[j+1]
	sw	2	6	array	mem[array+j+1] = array[j]
	lw	0	2	pos1	reg2 = 1 (restore)
outif	add	4	2	4	j++
	beq	0	0	iloopb
iloopa	add	3	2	3	i++
	beq	0	0	oloop	go back to outer for-loop
done	halt				terminate program
pos1	.fill	1
N	.fill	3
array	.fill	5
	.fill	3
	.fill	1
temp	.fill	0
test 	.fill	2147483647

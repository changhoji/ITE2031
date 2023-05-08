 	lw	0	0	zero	reg0 = 0
	lw	0	2	arr	reg2 = &arr
	lw	0	3	len	reg3 = len
	lw	0	5	sumAd	reg5 = &sum
	jalr	5	6	
done	halt
sum	lw	0	5	pos1	reg5 = 1
	lw	0	1	zero 	answer = 0
	lw	0	4	zero	i = 0
loop	beq	4	3	end	if i == len
	lw	4	7	arr	r7 = arr[i]
	add	1	7	1	answer += arr[i]
	add	4	5	4	i += 1
	beq	0	0	loop	jump to loop
end	jalr	6	5
zero	.fill	0
pos1	.fill	1
len	.fill	3
arr	.fill	2002
	.fill	8
	.fill	14
sumAd	.fill	sum

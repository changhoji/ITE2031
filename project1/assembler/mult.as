        lw      0       7       mcand   load reg6 with mcand
	lw      0	6	num15   load reg7 with '15'
	lw      0       5       neg1 	load reg4 with -1
	lw      0       4       pos1	load reg4 with 1
loop1	lw      0       2       mplier  load reg2 with mplier
 	add	0	0	3	save 0 to reg3 (mplier shift cnt)
loop2	add	2	2	2 	mplier << 1
	add     3	4	3 	mplier shift cnt ++
	beq     3	6	sum	finish mplier shift
	beq  	0	0	loop2	redo mplier shift
sum	lw      0       3       test	load reg3 with test
	nor     3	2	2	reg2 = mplier(shifted) nor test
	lw	0	3	test2	load reg3 with test2
	beq 	2	3	afadd	if mplier bit is 0, not add
	add	1	7	1       add mcand(shifted) to result
	beq	0	0	afadd	branch to after add
afadd   add     7       7       7       shift mcanf
        add     6       5       6       for amount--
	beq     6	0       end  	continue if mplier shift is done
	beq 	0	0	loop1
end	halt
mcand   .fill   32766
mplier  .fill   32767
pos1    .fill   1
neg1    .fill   -1
num15   .fill   15
test    .fill	-32769
test2   .fill	32768

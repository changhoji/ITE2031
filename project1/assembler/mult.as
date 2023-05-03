        lw      0       2       mcand   load reg2 with mcand
	lw      0	4	num15   load reg4 with '15'
loop1	lw      0       3       mplier  load reg3 with mplier
 	add	0	0	5	save 0 to reg5

	beq     4	5       loop1   continue if mplier shift is done

mcand   .fill   32766
mplier  .fill   12328
neg1    .fill   -1
num15   .fill   15

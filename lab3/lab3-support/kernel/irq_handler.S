#include <asm.h>

	.file 	"irqHandler.S"

FUNC(irqHandler)
	AND	r2, OSSR, #33554432
	CMP	r2, #0
	BE	done
	ADD	OSMR0, OSCR, #3250
	ORR	OSSR, OSSR, #33554432
	MOV	r2, =global_time
	MOV	r3, [r2]
	ADD	r3, r3, #10
	MOV	[r2], r3

done:	MOV	pc, lr
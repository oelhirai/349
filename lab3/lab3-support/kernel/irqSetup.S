/**
 Shashwat, Omer 10th November 2014
 */

#include <asm.h>

	.file	"irqSetup.S"

FUNC(irqSetup)
	MOV	r2, #33554432
	ORR	OIER, r2
	ADD	OSMR0, OSCR, #3250
	/*
		Might have to setup irq stack?
	 */
	MOV	pc, lr
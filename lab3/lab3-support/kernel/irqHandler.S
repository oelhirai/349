	.extern C_IRQ_Handler

	.file	"irqHandler.S"
	.text

	.global	irqHandler

irqHandler:
    ldr sp, =0xa2000000
	stmfd 	sp!, {r0-r12, lr}
	bl 	C_IRQ_Handler
	ldmfd	sp!, {r0-r12, lr}
	movs	pc, lr

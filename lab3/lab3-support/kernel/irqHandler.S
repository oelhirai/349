	.extern C_IRQ_Handler

	.file	"irqHandler.S"
	.text

	.global	irqHandler

irqHandler:
    ldr     sp, =0xA2800000
	stmfd 	sp!, {r0-r12, lr}

	@ restore r8, base pointer for uboot API
    ldr r8, =global_data
    ldr r8, [r8]

	bl 	C_IRQ_Handler
	ldmfd	sp!, {r0-r12, lr}
	movs	pc, lr

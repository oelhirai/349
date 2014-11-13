	.extern C_IRQ_Handler

	.file	"irqHandler.S"
	.text

	.global	irqHandler

irqHandler:
    ldr     sp, =0xA2800000        @set up irq stack
	stmfd 	sp!, {r0-r12, lr}      @push all registers onto stack

	@ restore r8, base pointer for uboot API
    ldr r8, =global_data           @restore r8 so we can use the uboot api
    ldr r8, [r8]

	bl 	C_IRQ_Handler				@branch to our handler
	ldmfd	sp!, {r0-r12, lr}		@get resgisters from stack
	movs	pc, lr 					@branch back to user code

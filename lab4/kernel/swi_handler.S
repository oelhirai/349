@ swi_handler.S: SWI Handler
@
@ Author: Harry Q Bovik <hqbovik@andrew.cmu.edu>
@ Date:   Tue, 23 Oct 2007 11:20:33 -0400
@
@ Hint: To change the SWI handler in class, we need to accommodate a return
@   value from the C_SWI_Handler.

    .extern C_SWI_Handler

    .file   "swi_handler.S"
    .text

    .global swi_handler
swi_handler:
    stmfd   sp!, {lr} @ pc value
    stmfd   sp!, {r0-r3, r8, ip, lr}

    @ Set Param 2: pointer to parameters on stack
    mov     r1, sp

    @ unmask interrupt bits to ensure IRQ
    mrs     r2, cpsr
    bic     r2, r2, #0x080  @ Mask the flag bits.
    msr     cpsr, r2

    @ get spsr into general purpose (gp) registers
    mrs     r2, spsr

    @ store spsr below gp registers
    stmfd   sp!, {r2} @adjusted

    /* Recover user registers and save */
    stmfd    sp, {r0, sp, lr}^
    add      sp, sp, #-12

    @ Set Param 1: extract SWI and get SWI no. by bit-masking
    ldr     r0, [lr, #-4]
    bic     r0, r0, #0xff000000

    @ restore r8, base pointer for uboot API
    ldr r8, =global_data
    ldr r8, [r8]

    /* Call the SWI handler in C. */
    bl      C_SWI_Handler

    /* Restore registers.  Also restore CPSR. */
    ldmfd    sp, {r0, sp, lr}^
    add      sp, sp, #12
    
    ldmfd    sp!, {r2}
    msr     spsr, r2
    
    @ Discard the r0 on the stack (r0 has ret value).
    add     sp, sp, #4
    ldmfd    sp!, {r1-r3, r8, ip, lr, pc}^


@ user_setup.S: helper function written in asm called from kernel
@
@ Author: Anand Pattabiraman <apattabi@andrew.cmu.edu>
@ Date:   Wed, 29 Oct 2014
@
@ Hint:
@   CPSR:
@       N|Z|C|V| [27:9] |I|F|T|M[4:0]
@   I -> 1
@   F -> 1
@   T -> T
@   CPSR_SVC = 0x10011 -> CPSR_USER = 0x10000

#include "bits/swi.h"

    .extern user_setup_stack_ptr


    .file   "user_setup.S"
    .text

    .global user_setup
user_setup:
    @ Save gp registers, lr, and cpsr so that we can return to kernel main later.
    stmfd   sp!, {r1-r12, lr}
    mrs     r2, cpsr
    stmfd   sp!, {r2}

    @ Save the stack pointer in a global variable for exit_handler.S.
    ldr     r2, =user_setup_stack_ptr
    str     sp, [r2] @ *user_setup_stack_ptr = sp

    @ switch mode to IRQ
    @ change sp to 0xA2800000
    mrs     r2, cpsr
    bic     r2, #0x001
    msr     cpsr, r2
    ldr     sp, =0xA2800000
	
    @ change mode to USER
    mrs     r2, cpsr
    #orr     r2, r2, #0x0C0  @ Mask the flag bits.
    bic     r2, r2, #0x0C3  @ Change mode bits to user.
    msr     cpsr, r2

    @ set up user stack from spTop passed in from kernel.c
    mov     sp, r0

    @ go to user program
    mov     lr, pc
    ldr     pc, =0xa0000000
    swi     EXIT_SWI

    @@@ exit syscall essentially returns through here!

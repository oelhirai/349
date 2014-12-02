/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 */

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)
	str		r4, [r1]  /* begin saving r4-r11, sp, lr from current context */
	add		r1, #4
	str		r5, [r1]
	add		r1, #4
	str		r6, [r1]
	add		r1, #4
	str		r7, [r1]
	add		r1, #4
	str		r8, [r1]
	add		r1, #4
	str		r9, [r1]
	add		r1, #4
	str		r10, [r1]
	add		r1, #4
	str		r11, [r1]	
	add		r1, #4
	str		sp, [r1]	/* pointers assumed to be 32 bit values */
	add		r1, #4
	str		lr, [r1]

	ldr		r4, [r0]
	add		r0, #4
	ldr		r5, [r0]
	add		r0, #4
	ldr		r6, [r0]
	add		r0, #4
	ldr		r7, [r0]
	add		r0, #4
	ldr		r8, [r0]
	add		r0, #4
	ldr		r9, [r0]
	add		r0, #4
	ldr		r10, [r0]
	add		r0, #4
	ldr		r11, [r0]
	add		r0, #4
	ldr		sp, [r0]
	add		r0, #4
	ldr		lr, [r0]

	/* check if new task (cmp against dummy value) */
	ldr 	ip, =0xcafebabe
	cmp     lr, ip
	beq     launch_task

	mov 	pc, lr


/*add your code to perform a half context switch */	
FUNC(ctx_switch_half)
	/* load context of new task */
	ldr		r4, [r0]
	add		r0, #4
	ldr		r5, [r0]
	add		r0, #4
	ldr		r6, [r0]
	add		r0, #4
	ldr		r7, [r0]
	add		r0, #4
	ldr		r8, [r0]
	add		r0, #4
	ldr		r9, [r0]
	add		r0, #4
	ldr		r10, [r0]
	add		r0, #4
	ldr		r11, [r0]
	add		r0, #4
	ldr		sp, [r0]
	add		r0, #4
	ldr		lr, [r0]

	/* launch task */
	b     launch_task


/** @file event_wait.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <swi.h>
#include <asm.h>

	.file "event_wait.S"

FUNC(event_wait)
    swi CREATE_SWI @make the system call

    @ Return on success
    cmp r0, #0 @success
    mov pc, lr @pl means >= 0
    /* now not accessing code */
    @ Store the absolute value of return value in errno
    neg r0, r0
    ldr r1, =errno
    str r0, [r1]

    @ Store -1 in return value
    mov r0, #
    mov pc, lr

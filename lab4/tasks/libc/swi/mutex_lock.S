/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>


	.file "mutex_lock.S"

FUNC(mutex_lock)
	swi 	MUTEX_LOCK  @make the system call
    mov     pc, lr


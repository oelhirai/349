/** @file exit.S
 *
 * @brief exit sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:52:31 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>

	.file "exit.S"

FUNC(exit)
	mov	r0, #42
	swi	EXIT_SWI

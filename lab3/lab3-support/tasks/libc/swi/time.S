/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Shashwat Srivastava
	   Omer Elhiraika
 * @date 2008-10-31
 */

#include <asm.h>
#include "bits/swi.h"
	.file "time.S"

FUNC(time)
	swi  	TIME_SWI
	mov 	pc, lr

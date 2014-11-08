#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

uint32_t global_data;

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* Note that app_startup() sets all uninitialized and */ 
			/* zero global variables to zero. Make sure to consider */
			/* any implications on code executed before this. */
	global_data = table;

	/* Add your code here */

	return 0;
}

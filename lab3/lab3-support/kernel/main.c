#include <exports.h>

#include <exports.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <bits/swi.h>
#include <arm/reg.h>

#include "globals.h"
#include "swi_handler.h"
#include "user_setup.h"
#include "exit_handler.h"

typedef enum {FALSE, TRUE} bool;

// 0xe59ff014 (LDR pc, [pc, 0x14]) --> 0x014 through masking
#define SWI_VECT_ADDR 0x08
#define IRQ_VECT_ADDR 0x18
#define PC_OFFSET 0x08

// Cannot write to this address. kernel.bin loaded here. Stack grows down.
#define USER_STACK_TOP 0xa3000000

// (LDR pc, [pc, 0x000]) 0xe59ff000 --> 0x000 through masking
#define LDR_PC_PC_INSTR 0xe59ff000
#define LDR_SIGN_MASK 0x00800000

#define BAD_CODE 0x0badc0de

#define SFROM_START 0x00000000
#define SFROM_END 0x00ffffff
#define SDRAM_START 0xa0000000
#define SDRAM_END 0xa3ffffff

#define OIER 0xA0001C
#define OSCR 0xA00010
#define OSSR 0xA00014
#define OSMR0 0xA00000
#define ICMR 0xD00004
#define ICLR 0xD00008
// Stores how much time has passed since program started
unsigned long global_time;

// Call to irqHandler assembly function
extern void irqHandler();

// Sets up IRQ stuff
void IRQ_setup();

// IRQ handler
void C_IRQ_Handler();

/* Checks the SWI Vector Table. */
bool check_vector(int addr) {
	int swi_vector_instr = *((int *)addr);

	// Check if the offset is negative.
	if ((swi_vector_instr & LDR_SIGN_MASK) == 0) {
		return FALSE;
	}

	// Check that the instruction is a (LDR pc, [pc, 0x000])
	if ((swi_vector_instr & 0xFFFFF000) != LDR_PC_PC_INSTR) {
		return FALSE;
	}

	return TRUE;
}

uint32_t global_data;

int* IRQ_SWI_addr(int initAddr)
{
  /* Add your code here */
  if (check_vector(initAddr) == FALSE) {
    return (int*) BAD_CODE;
  }
  
  /** Wire in the SWI handler. **/
  // Jump offset already incorporates PC offset. Usually 0x10 or 0x14.
  int jmp_offset = (*((int *) SWI_VECT_ADDR))&(0xFFF);

  return  *(int **)(SWI_VECT_ADDR + PC_OFFSET + jmp_offset);
}

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* Note that app_startup() sets all uninitialized and */ 
	/* zero global variables to zero. Make sure to consider */
	/* any implications on code executed before this. */
	global_data = table;
	global_time = 0;

	// &S_Handler" in Jump Table.
	int *swi_handler_addr = IRQ_SWI_addr(SWI_VECT_ADDR);
	int *irq_handler_addr = IRQ_SWI_addr(IRQ_VECT_ADDR);
	if (*swi_handler_addr == BAD_CODE || 
		*irq_handler_addr == BAD_CODE) {
		return BAD_CODE;
	}

	// Save original Uboot SWI handler instructions.
	int swi_instr_1 = *swi_handler_addr;
	int swi_instr_2 = *(swi_handler_addr + 1);

	int irq_instr_1 = *irq_handler_addr;
	int irq_instr_2 = *(irq_handler_addr + 1);

	// Wire in our own: LDR pc, [pc, #-4] = 0xe51ff004
	*swi_handler_addr = 0xe51ff004;
	*(swi_handler_addr + 1) = (int) &swi_handler; // New swi handler.

	*irq_handler_addr = 0xe51ff004;
	*(irq_handler_addr + 1) = (int) &irqHandler;

	// Copy argc and argv to user stack in the right order.
	int *spTop = ((int *) USER_STACK_TOP) - 1;
	int i = 0;
	for (i = argc-1; i >= 0; i--) {
		*spTop = (int)argv[i];
		spTop--;
	}
	*spTop = argc;
	
	/* Setup IRQ values */
	IRQ_setup();

	/** Jump to user program. **/
	int usr_prog_status = user_setup(spTop);


	/** Restore SWI Handler. **/
	*swi_handler_addr = swi_instr_1;
	*(swi_handler_addr + 1) = swi_instr_2;

	*irq_handler_addr = irq_instr_1;
	*(irq_handler_addr + 1) = irq_instr_2;

	return usr_prog_status;
}

// Sets up stack pointers
void IRQ_setup()
{
  int mask = 1 << 25;
  reg_set(OIER, mask);
  int timeNow = reg_read(OSCR);
  reg_write(OSMR0, timeNow + 325000);
  reg_set(ICMR, mask);
  reg_clear(ICLR, mask);
}

/* Verifies that the buffer is entirely in valid memory. */
int check_mem(char *buf, int count, unsigned start, unsigned end) {
	unsigned start_buf = (unsigned) buf;
	unsigned end_buf = (unsigned)(buf + count);

	if ( (start_buf < start) || (start_buf > end) ) {
		return FALSE;
	}
	if ( (end_buf < start) || (end_buf > end) ) {
		return FALSE;
	}
	// Overflow case.
	if ( start_buf >= end_buf ) {
		return FALSE;
	}

	return TRUE;
}

// write function to replace the system's write function
ssize_t write_handler(int fd, const void *buf, size_t count) {

	// Check for invalid memory range or file descriptors
	if (check_mem((char *) buf, (int) count, SDRAM_START, SDRAM_END) == FALSE &&
			check_mem((char *) buf, (int) count, SFROM_START, SFROM_END) == FALSE) {
		exit_handler(-EFAULT);
	} else if (fd != STDOUT_FILENO) {
		exit_handler(-EBADF);
	}

	char *buffer = (char *) buf;
	size_t i;
	char read_char;
	for (i = 0; i < count; i++) {
		// put character into buffer and putc
		read_char = buffer[i];
		putc(read_char);
	}
	return i;
}


// read function to replace the system's read function
ssize_t read_handler(int fd, void *buf, size_t count) {
	// Check for invalid memory range or file descriptors
	if (check_mem((char *) buf, (int) count, SDRAM_START, SDRAM_END) == FALSE) {
		exit_handler(-EFAULT);
	} else if (fd != STDIN_FILENO) {
		exit_handler(-EBADF);
	}

	size_t i = 0;
	char *buffer = (char *) buf;
	char read_char;

	while (i < count) {
		read_char = getc();

		if (read_char == 4) { //EOT character
			return i;
		} else if (((read_char == 8) || (read_char == 127))) { // backspace or DEL character
			buffer[i] = 0; // '\0' character
			if(i > 0) {
				i--;
				puts("\b \b");
			}
		} else if ((read_char == 10) || (read_char == 13)) { // '\n' newline or '\r' carriage return character
			buffer[i] = '\n';
			putc('\n');
			return (i+1);
		} else {
			// put character into buffer and putc
			buffer[i] = read_char;
			i++;
			putc(read_char);
		}
	}

	return i;
}

void C_IRQ_Handler()
{
  int timeNow = reg_read(OSCR);
  int i;
  // Use 3250 later
  reg_write(OSMR0, timeNow + 325000);
  global_time += 10;
  int mask = 1 << 25;
  reg_set(OSSR, mask);
  for(i = 0; i < 3; i++)
  {
  	putc('a');
  	putc('b');
  }
  putc('\n');
}

/* C_SWI_Handler uses SWI number to call the appropriate function. */
int C_SWI_Handler(int swiNum, int *regs) {
	int count = 0;
	unsigned long timeNow, timeSleep;
	switch (swiNum) {
		// ssize_t read(int fd, void *buf, size_t count);
		case READ_SWI:
			count = read_handler(regs[0], (void *) regs[1], (size_t) regs[2]);
			break;
			// ssize_t write(int fd, const void *buf, size_t count);
		case WRITE_SWI:
			count = write_handler((int) regs[0], (void *) regs[1], (size_t) regs[2]);
			break;
			// void exit(int status);
		case EXIT_SWI:
			exit_handler((int) regs[0]); // never returns
			break;
	case TIME_SWI: 
			return (int)global_time;
		    break;
	case SLEEP_SWI: 
			timeNow = global_time;
	        timeSleep = (unsigned long)regs[0];
	        while(global_time - timeNow < timeSleep);
	        break;
		default:
			printf("Error in ref C_SWI_Handler: Invalid SWI number.");
			exit_handler(BAD_CODE); // never returns
	}

	return count;
}


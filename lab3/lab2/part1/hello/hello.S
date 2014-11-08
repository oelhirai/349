@ hello.S: Hello world! in assembly
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date

#include <bits/swi.h>
#include <bits/fileno.h>

	.file	"hello.S"
    .data
hello:
    .asciz "Hello world!\n"
    .equ len, . - hello
    .text
	.global	_start
_start:
    ldr r0, [sp]   @ give argc
    add r1, sp, #4 @ give argv
    bl main
    swi EXIT_SWI
main:
    mov r0, #0
    ldr r1, =hello
    ldr r2, =len
    swi WRITE_SWI
    mov pc, lr

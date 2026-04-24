.syntax unified
.cpu cortex-m4
.thumb

.global _estack
.global Reset_Handler
.global SysTick_Handler

/* Top of stack (adjust if needed) */
_estack = 0x20020000   /* 128 KB RAM end for STM32F411 */

/* Vector table */
.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word _estack            /* Initial stack pointer */
    .word Reset_Handler      /* Reset handler */

    /* Minimal handlers (fill with default) */
    .word Default_Handler    /* NMI */
    .word Default_Handler    /* HardFault */
    .word Default_Handler    /* MemManage */
    .word Default_Handler    /* BusFault */
    .word Default_Handler    /* UsageFault */
    .word 0                  /* Reserved */
    .word 0
    .word 0
    .word 0
    .word Default_Handler    /* SVC */
    .word Default_Handler    /* DebugMon */
    .word 0                  /* Reserved */
    .word Default_Handler    /* PendSV */
    .word SysTick_Handler    /* SysTick */
    /* External interrupts (just fill with default) */
    .rept 82
    .word Default_Handler
    .endr

.size g_pfnVectors, . - g_pfnVectors

/* Reset handler */
.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    /* Copy .data from flash to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcc copy_word
    b init_bss

copy_word:
    ldr r3, [r0], #4
    str r3, [r1], #4
    b copy_data

/* Zero initialize .bss */
init_bss:
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0

zero_bss:
    cmp r1, r2
    bcc zero_word
    b call_main

zero_word:
    str r3, [r1], #4
    b zero_bss

/* Call main */
call_main:
    bl main

/* If main returns, loop forever */
hang:
    b hang

.size Reset_Handler, . - Reset_Handler

/* Default interrupt handler */
.section .text.Default_Handler
.type Default_Handler, %function

Default_Handler:
    b .

.size Default_Handler, . - Default_Handler

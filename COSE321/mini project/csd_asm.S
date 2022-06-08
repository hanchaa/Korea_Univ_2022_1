#include "csd_zynq_peripherals.h"
#include "uart_init.s"

#define	TIMER_INITIAL	30302

.align 5

csd_vector_table:
	b .
	b .
	b .
	b .
	b .
	b .
	b csd_IRQ_ISR
	b .

.extern task1_c
.extern task2_c
.extern task3_c

.global main
main:

	// Disable interrupt: CPSR'I = 1
	cpsID i

	cps #0x12	   	       // IRQ mode 
	ldr	r13,=irq_stack_top // Stack pointer setup for IRQ mode

	cps #0x13		          // supervisor mode

	// task1 pcb and stack setup
	ldr r1, =task1_c
	add r1, r1, #4
	ldr r2, =task1_stack_top
	ldr r3, =task1_pcb_pointer
	str r1, [r3, #-4]
	str r2, [r3, #52]
	// set task1 spsr as user mode
	mov r1, #0x10
	str r1, [r3, #-8]

	// task2 pcb and stack setup
	ldr r1, =task2_c
	add r1, r1, #4
	ldr r2, =task2_stack_top
	ldr r3, =task2_pcb_pointer
	str r1, [r3, #-4]
	str r2, [r3, #52]
	// set task2 spsr as user mode
	mov r1, #0x10
	str r1, [r3, #-8]

	// task3 pcb and stack setup
	ldr r1, =task3_c
	add r1, r1, #4
	ldr r2, =task3_stack_top
	ldr r3, =task3_pcb_pointer
	str r1, [r3, #-4]
	str r2, [r3, #52]
	// set task3 spsr as user mode
	mov r1, #0x10
	str r1, [r3, #-8]

	ldr	r13,=svc_stack_top // Stack pointer setup for SVC mode

	cps #0x1F	             // SYS mode 

	// Set VBAR (Vector Base Address Register) to my vector table
	ldr     r0, =csd_vector_table
	mcr     p15, 0, r0, c12, c0, 0
	dsb
	isb

	// Enable interrupt: CPSR'I = 0
	cpsIE i

// ---------------------------
// Generic Interrupt Controller (GIC) setup - Begin
// ---------------------------

	// Distributor Control Register
	ldr r0, =GICD_CTLR
	ldr r1, [r0]
	mov r2, #1       // Enable
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	 // Interrupt Set-Enable Register 0
	ldr r0, =GICD_ISENABLER0
	ldr r1, [r0]
	mov r2, #1 << 29   // Enable #29 (Private Timer)
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	// According to specifications, 
	// Cortex-A9 supports 5-bit version of priority format [7:3] in secure world
	// ( 0 -> 8 -> 16 -> 24...)

	// Interrupt Priority Register #7
	ldr r0, =GICD_PRIOR7
	ldr r1, [r0]
	mov r2, #0x10 << 8    // Priority 16 for ID# 29 (Private Timer)
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	// CPU Interface Control Register
	ldr r0, =GICC_CTLR
	ldr r1, [r0]
	mov r2, #1        // Enable
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

 	// CPU Interface Interrupt Priority Mask Register
	ldr r0, =GICC_PMR
	ldr r1, [r0]
	mov r2, #0xFF     // Lowest
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

// ---------------------------
// Generic Interrupt Controller (GIC) setup - End
// ---------------------------


// ---------------------------
// Private Timer setup - Begin
// ---------------------------

 	// Private Timer Load Register
	ldr r0, =PRIVATE_LOAD
 	ldr r1, =TIMER_INITIAL
 	str r1, [r0]

 	// Private Timer Control Register
  	ldr r0, =PRIVATE_CONTROL
 	mov r1, #10 << 8   // Prescalar
 	orr r1, r1, #7     // IRQ Enable, Auto-Reload, Timer Enable
 	str r1, [r0]

// ----------------------------
// Private Timer setup - End
// ----------------------------
	UART_init

	cps #0x10
	ldr sp, =task1_stack_top
	b task1_c

// ----------------------------
// Interrupt Service Routines (ISRs) - Begin
// ----------------------------

csd_IRQ_ISR:
	stmfd sp!, {r0-r12}
	mrs r0, spsr
	stmfd sp!, {r0, lr}

	// interrupt ack
	ldr r0, =GICC_IAR
	ldr r1, [r0]

	// clear timer interrupt status bit
	ldr r0, =PRIVATE_STATUS
	mov r2, #1
	str r2, [r0]

	// end of interrupt
	ldr r0, =GICC_EOIR
	str r1, [r0]

	ldr r0, =current_task
	ldr r1, [r0]

	cmp r1, #1
	beq task2

	cmp r1, #2
	beq task3

	cmp r1, #3
	beq task1

// begin task 1
task1:
	// change current task to 1
	mov r1, #1
	str r1, [r0]

	// save registers to pcb
	ldmfd sp!, {r0, r1}
	ldr r2, =task3_pcb_pointer
	stmdb r2, {r0, r1}

	ldmfd sp!, {r0-r12}

	ldr sp, =task3_pcb_pointer
	stmia sp, {r0-lr}^

	// load registers from pcb
	ldr sp, =task1_pcb_pointer
	ldmdb sp, {r0, lr}
	msr spsr, r0
	ldmia sp, {r0-lr}^

	// restore irq sp
	ldr sp, =irq_stack_top

	subs pc, lr, #4


// begin task 2
task2:
	// change current task to 2
	mov r1, #2
	str r1, [r0]

	// save registers to pcb
	ldmfd sp!, {r0, r1}
	ldr r2, =task1_pcb_pointer
	stmdb r2, {r0, r1}

	ldmfd sp!, {r0-r12}

	ldr sp, =task1_pcb_pointer
	stmia sp, {r0-lr}^

	// load registers from pcb
	ldr sp, =task2_pcb_pointer
	ldmdb sp, {r0, lr}
	msr spsr, r0
	ldmia sp, {r0-lr}^

	// restore irq sp
	ldr sp, =irq_stack_top

	subs pc, lr, #4

// begin task 3
task3:
	// change current task to 3
	mov r1, #3
	str r1, [r0]

	// save registers to pcb
	ldmfd sp!, {r0, r1}
	ldr r2, =task2_pcb_pointer
	stmdb r2, {r0, r1}

	ldmfd sp!, {r0-r12}

	ldr sp, =task2_pcb_pointer
	stmia sp, {r0-lr}^

	// load registers from pcb
	ldr sp, =task3_pcb_pointer
	ldmdb sp, {r0, lr}
	msr spsr, r0
	ldmia sp, {r0-lr}^

	// restore irq sp
	ldr sp, =irq_stack_top

	subs pc, lr, #4
// ----------------------------
// Interrupt Service Routines (ISRs) - End
// ----------------------------

.data
.align 4

user_stack:    .space 1024
user_tack_top:
irq_stack:     .space 1024
irq_stack_top:
svc_stack:     .space 1024
svc_stack_top:

task1_stack:   .space 1024
task1_stack_top:
task2_stack:   .space 1024
task2_stack_top:
task3_stack:   .space 1024
task3_stack_top:

task1_pcb:     .space 8
task1_pcb_pointer: .space 60
task2_pcb:     .space 8
task2_pcb_pointer: .space 60
task3_pcb:     .space 8
task3_pcb_pointer: .space 60

current_task: .word 1

#include "csd_zynq_peripherals.h"
#include "uart_init.s"

#define TIMER_INITIAL 1999999
#define PRE_SCALER #165

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

.global main
main:

	// Disable interrupt: CPSR'I = 1
	cpsID i

	cps #0x12	   	       // IRQ mode 
	ldr	r13,=irq_stack_top // Stack pointer setup for IRQ mode

	cps #0x13		          // supervisor mode
	ldr	r13,=svc_stack_top // Stack pointer setup for SVC mode

	cps #0x11		          // FIQ mode
	ldr	r13,=fiq_stack_top // Stack pointer setup for FIQ mode

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

	// CPU Interface ID Register
	ldr r0, =GICC_IIDR
	ldr r3, [r0]

	// CPU Controller Type Register
	ldr r0, =GICD_TYPER
	ldr r3, [r0]

	// CPU Binary Pointer Register
	ldr r0, =GICC_BPR
	ldr r3, [r0]

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

	// init uart
	UART_init

// ---------------------------
// Private Timer setup - Begin
// ---------------------------

 	// Private Timer Load Register
	ldr r0, =PRIVATE_LOAD
 	ldr r1, =TIMER_INITIAL // clock out인 2Mhz에 맞게 timer_initial + 1 = 2000000가 되도록 설정
 	str r1, [r0]

 	// Private Timer Control Register
  	ldr r0, =PRIVATE_CONTROL
 	mov r1, PRE_SCALER << 8 // clock out이 2Mhz가 되도록 pre scaler 설정
 	orr r1, r1, #7     // IRQ Enable, Auto-Reload, Timer Enable
 	str r1, [r0]

	// 00:00:00 출력
	ldr r2, =timer_string
	bl send_uart
// ----------------------------
// Private Timer setup - End
// ----------------------------

 	// Check out the counter value to make sure the counter is decrementing
	ldr r0, =PRIVATE_COUNTER
	ldr r1, [r0]
	ldr r2, [r0]
	ldr r3, [r0]
	ldr r4, [r0]
	ldr r5, [r0]
	ldr r6, [r0]
	ldr r7, [r0]
	ldr r8, [r0]

	ldr r0, =PRIVATE_COUNTER
	ldr r1, =PRIVATE_STATUS
	ldr r2, =GICD_ISPENDR0

forever:
	ldr r5, [r0]
	ldr r6, [r1]
	ldr r7, [r2]
	b forever



// ----------------------------
// Interrupt Service Routines (ISRs) - Begin
// ----------------------------

csd_IRQ_ISR:

	stmfd sp!, {r0-r12, lr}

 	// Interrupt Ack
  	ldr r0, =GICC_IAR
	ldr r3, [r0]

	stmfd sp!, {r3}
	bl increase_timer
	ldr r2, =timer_string
	bl send_uart
	ldmfd sp!, {r3}

 	// Clear Interrupt Status bit
  	ldr r0, =PRIVATE_STATUS
  	mov r1, #1
	str r1, [r0]

 	// End-of-Interrupt
  	ldr r0, =GICC_EOIR
	str r3, [r0]

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr, #4

increase_timer:
	ldr r0, =timer_string
	add r0, r0, #8

	// second 1의 자리 수
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58 // 9의 아스키 코드=57
	moveq r1, #48
	strb r1, [r0], #-1
	movne pc, lr // 올림이 없으면 increase 중단

	// second 10의 자리 수
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #54 // 5의 아스키 코드=53
	moveq r1, #48
	strb r1, [r0], #-2
	movne pc, lr // 올림이 없으면 increase 중단

	// minute 1의 자리 수
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58 // 9의 아스키 코드=57
	moveq r1, #48
	strb r1, [r0], #-1
	movne pc, lr // 올림이 없으면 increase 중단

	// minute 10의 자리 수
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #54 // 5의 아스키코드=53
	moveq r1, #48
	strb r1, [r0], #-2
	movne pc, lr // 올림이 없으면 increase 중단

	// hour 1의 자리 수
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58 // 9의 아스키 코드=57
	moveq r1, #48
	strb r1, [r0], #-1
	movne pc, lr // 올림이 없으면 increase 중단

	// hour 10의 자리 수
	ldrb r1, [r0]
	add r1, r1, #1
	cmp r1, #58 // 9의 아스키 코드=57
	moveq r1, #48
	strb r1, [r0], #-1

	mov pc, lr

send_uart:
	// tx buffer가 full인지 확인 후 그렇다면 반복
	ldr r0, =uart_Channel_sts_reg0
	ldr r1, [r0]
	and r1, r1, #0b10000
	cmp r1, #0b10000
	beq send_uart

	ldr r0, =uart_TX_RX_FIFO0
	ldrb r3, [r2], #1
	strb r3, [r0]

	// null 문자를 만나면 전송 중단
	cmp r3, #0
	moveq pc, lr

	b send_uart

// ----------------------------
// Interrupt Service Routines (ISRs) - End
// ----------------------------

.data
.align 4

irq_stack:     .space 1024
irq_stack_top:
fiq_stack:     .space 1024
fiq_stack_top:
svc_stack:     .space 1024
svc_stack_top:

timer_string:
	.byte 0x0D
	.ascii "00"
	.ascii ":"
	.ascii "00"
	.ascii ":"
	.ascii "00"
	.byte 0x00

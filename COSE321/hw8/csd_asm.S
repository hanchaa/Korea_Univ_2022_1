#define SW_ADDR 0x41210000
#define LED_ADDR 0x41200000
#include "csd_zynq_peripherals.h"

.extern csd_main

.global main
main:
	ldr r0, =LED_ADDR
	mov r1,  #0xC3
	strb r1, [r0]

forever:
	// GPIO의 스위치 값 읽기
	ldr r0, =SW_ADDR
	ldrb r1, [r0]
	
	// 첫 번째 스위치가 내려가 있으면 cache disable, 올라가 있으면 cache enable
	and r1, r1, #0b1
	cmp r1, #0
	bleq disable_cache
	blne enable_cache

	// loop를 돌면서 일정 시간 동안 대기
	bl  csd_main

	// LED toggle
	ldr r0, =LED_ADDR
	ldrb r1, [r0]
	eor r1, r1, #0xFF
	strb r1, [r0]

	b forever

disable_cache:
	@------------------------
	@ Disable Caches (L2)
	@------------------------
	ldr r0, =L2_reg1_ctrl
	mov r1, #0x0
	str r1, [r0]
	@------------------------
	@ Disable Caches (IL1, DL1)
	@------------------------
	mrc		p15, 0, r0, c1, c0, 0	@ read control register (CP15 register1)
	bic		r0, r0, #4096		    @ disable I bit (Instruction Cache)
	bic		r0, r0, #4		        @ disable C bit (Data and Unified Caches)
	mcr		p15, 0, r0, c1, c0, 0	@ write control register (CP15 register2)

	mov pc, lr

enable_cache:
	@------------------------
	@ Enable Caches (L2)
	@------------------------
	ldr r0, =L2_reg1_ctrl
    mov r1, #0x1
    str r1, [r0]

	@------------------------
	@ Enable Caches (IL1, DL1)
	@------------------------
	mrc		p15, 0, r0, c1, c0, 0	@ read control register (CP15 register1)
	orr		r0, r0, #(1<<12)	    @ Enable I bit (Instruction Cache)
	orr		r0, r0, #(1<<2)         @ Enable C bit (Data and Unified Caches)
	mcr		p15, 0, r0, c1, c0, 0	@ write control register (CP15 register2)

	mov pc, lr


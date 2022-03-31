#define csd_LED_ADDR 0x41200000

.extern csd_main

.global main

main:
    ldr r0, =csd_LED_ADDR
    mov r1, #0b1

loop:
	cmp r1, #0b100000000
	moveq r1, #0b1

	strb r1, [r0]

	bl csd_main

	lsl r1, #1
	b loop

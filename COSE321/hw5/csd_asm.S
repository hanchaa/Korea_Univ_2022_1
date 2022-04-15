#include "uart_init.s"
#include "debug_macro.s"
#define csd_LED_ADDR 0x41200000

.global main

main:
	UART_init
	DEBUG_macro

    ldr r0, =Input_data 	// Input_data의 메모리 주소
    ldr r1, =Output_data 	// Ouput_data의 메모리 주소
    ldr r2, =32 			// 총 단어의 개수

	DEBUG_macro

move_data: 					// Input_data를 Output_data 위치로 이
	ldr r3, [r0], #4 		// Input_data 한 개 load
	str r3, [r1], #4 		// load한 데이터를 Output_data에 저장

	sub r2, r2, #1 			// 데이터 개수 카운트 감소
	cmp r2, #0 				// 모든 데이터를 옮겼는지 확인
	bne move_data			// 데이터를 덜 옮겼으면 다시 move_data label로 이동

// 버블 정렬 알고리즘 이용
// L0 ~ L2 outer loop
// L1 inner loop
	ldr r0, =32				// r0 = outer loop 카운터
L0:
	cmp r0, #0
	beq L3					// outer loop를 모두 다 돌았으면 L4로 이동

	sub r1, r0, #1			// r1 = inner loop 카운터

	ldr r2, =Output_data	// r2 = output_data의 주소

L1:
	cmp r1, #0
	beq L2					// 데이터 개수 - outer loop 수행 횟수 - 1 만큼 inner loop를 돌면 inner loop 종료

	ldr r3, [r2], #4		// data[i]
	ldr r4, [r2]			// data[i+1]

	cmp r3, r4
	strgt r4, [r2, #-4]
	strgt r3, [r2]			// data[i] > data[i+1]이면 자리 바꿔서 메모리에 저장

	sub r1, r1, #1			// inner loop 카운터 감소
	b L1					// inner loop 반복

L2:
	sub r0, r0, #1			// outer loop 카운터 감소
	b L0					// outer loop 반복

L3:
    nop

.data
.align 4
Input_data: .word 2, 0, -7, -1, 3, 8, -4, 10
            .word -9, -16, 15, 13, 1, 4, -3, 14
            .word -8, -10, -15, 6, -13, -5, 9, 12
            .word -11, -14, -6, 11, 5, 7, -2, -12
Output_data: .word 0, 0, 0, 0, 0, 0, 0, 0
             .word 0, 0, 0, 0, 0, 0, 0, 0
             .word 0, 0, 0, 0, 0, 0, 0, 0
             .word 0, 0, 0, 0, 0, 0, 0, 0

seperator:
	.ascii "-----------------------------------------------------------------------------------------------------"
	.byte 0x0D
	.byte 0x0A
	.byte 0x00

cpsr:
	.ascii "cpsr = "
	.byte 0x00

arm_mode:
	.ascii "ARM mode, current mode = "
	.byte 0x00

thumb_mode:
	.ascii "Thumb mode, current mode = "
	.byte 0x00

jazelle_mode:
	.ascii "Jazelle mode, current mode = "
	.byte 0x00

thumbee_mode:
	.ascii "ThumbEE mode, current mode = "
	.byte 0x00

user_mode:
	.ascii "USR ( =0x"
	.byte 0x00

fiq_mode:
	.ascii "FIQ ( =0x"
	.byte 0x00

irq_mode:
	.ascii "IRQ ( =0x"
	.byte 0x00

svc_mode:
	.ascii "SVC ( =0x"
	.byte 0x00

monitor_mode:
	.ascii "MON ( =0x"
	.byte 0x00

abort_mode:
	.ascii "ABT ( =0x"
	.byte 0x00

hyp_mode:
	.ascii "HYP ( =0x"
	.byte 0x00

undefined_mode:
	.ascii "UND ( =0x"
	.byte 0x00

system_mode:
	.ascii "SYS ( =0x"
	.byte 0x00

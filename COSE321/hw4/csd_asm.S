#include "uart_init.s"
#define csd_LED_ADDR 0x41200000

.extern csd_main

.global main

main:
	UART_init    						// UART Initialization

	mov r0, #8			  				// 기본 간격을 1초로 설정
	mov r1, #0b1		  				// 몇 번째 led를 켜야하는지 저장
	ldr r2, =csd_LED_ADDR 				// LED가 mapping 된 주소 불러오기
	ldr r3, =string						// prompt가 저장된 string의 주소 불러오기
	ldr r4, =uart_Channel_sts_reg0		// uart channel의 상태 레지스터가 mapping 된 주소 불러오기
	ldr r5, =uart_TX_RX_FIFO0			// TX RX FIFO buffer가 mapping된 주소 불러오기

transmit:
	ldr r6, [r4]						// uart channel의 상태 레지스터 불러오기
	// Tx FIFO가 비어있는지 확인
	and r6, r6, #0x8
	cmp r6, #0x8

	bne transmit						// 비어있지 않다면 비어질 때까지 대기

	ldrb r6, [r3], #1					// string의 한 글자 읽기
	strb r6, [r5]						// string의 한 글자 전송하기

	cmp r6, #0							// null 문자까지 전송하였는지 확인
	bne transmit					// 모든 string을 전송할 때까지 반복
	ldr r3, =string

receive:
	ldr r6, [r4]						// uart channel의 상태 레지스터 불러오기
	// Rx FIFO가 비어있는지 확인
	and r6, r6, #0x2
	cmp r6, #0x2

	beq led								// 비어있다면 led를 킴

	// UART로 전송된 문자를 그대로 보내줘서 echo 구현
	ldr r6, [r5]
	str r6, [r5]

	sub r0, r6, #48						// csd_main 함수의 parameter로 uart로 전송받은 숫자 이용, ascii 숫자를 정수로 바꿈

	// teraterm에서 줄바꿈 수행
	mov r6, #0xD
	str r6, [r5]
	mov r6, #0xA
	str r6, [r5]

	b transmit

led:
	// 8번째 led까지 모두 돌아갔다면 다시 첫번째 led를 킴
	cmp r1, #0b100000000
	moveq r1, #0b1

	strb r1, [r2]	// led 점등

	bl csd_main		// csd_main 함수로 이동해 지정된 시간 만큼 대기, 이때 r0에 있는 값을 파라미터로 사용

	ldr r2, =csd_LED_ADDR 				// 함수 실행으로 인해 변경된 r2에 LED가 mapping 된 주소 다시 불러오기
	ldr r3, =string						// 함수 실행으로 인해 변경된 r3에 prompt가 저장된 string의 주소 다시 불러오기

	lsl r1, #1		// 다음 led 점등하도록 값 변경
	b receive

.data
.align 4

string:
	.ascii "----------------- LED On Duration ----------------"
	.byte 0x0D
	.byte 0x0A
	.ascii "1. 100ms 2. 200ms 3. 300ms 4. 400 ms"
	.byte 0x0D
	.byte 0x0A
	.ascii "5. 500ms 6. 600ms 7. 700ms 8. 1 sec"
	.byte 0x0D
	.byte 0x0A
	.ascii "---------------------------------------------------"
	.byte 0x0D
	.byte 0x0A
	.ascii "Select: "
	.byte 0x00

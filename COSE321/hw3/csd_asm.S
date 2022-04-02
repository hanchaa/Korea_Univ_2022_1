#define csd_LED_ADDR 0x41200000

.extern csd_main

.global main

main:
    ldr r0, =csd_LED_ADDR 		@ LED의 메모리 주소 load
    mov r1, #0b1			@ r1에 몇 번째 led를 켤지 저장

loop:
	cmp r1, #0b100000000
	moveq r1, #0b1		@ 8번째 led까지 돌아갔는지 확인 후 맞다면 1번째 led 켜기

	strb r1, [r0]			@ LED의 메모리 주소에 몇 번째 led를 켤지 store

	bl csd_main			@ 스위치의 상태에 따라 일정 시간 대기

	lsl r1, #1			@ 다음 led가 켜지도록 왼쪽으로 한 번 쉬프트
	b loop				@ 무한 반복

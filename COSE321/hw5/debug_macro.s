#include "uart_regs.h"

.macro DEBUG_macro

stmfd sp!, {r0-r12} 	// �������͵� ���ÿ� ���
add r0, sp, #52			// ������ ���������� r0�� ����
mov r1, lr				// lr �������� r1�� ����
sub r2, pc, #24			// ������ pc r2�� ����
mrs r3, cpsr			// r3�� cpsr ����

ldr r11, =uart_Channel_sts_reg0
ldr r12, =uart_TX_RX_FIFO0

ldr r4, =13f // seperator		// ���м� ���
bl 1f // print_string


ldr r5, =10f // backup_registers // �������� ���� ����� �޸� �ּ�
mov r7, #0				// �� ��° ������������ ����
bl 3f // loop_register

// cpsr ���
ldr r4, =14f // cpsr
bl 1f // print_string

// n
ldr r4, =0x80000000
and r5, r3, r4
cmp r5, r4
ldreq r9, =0x4e
ldrne r9, =0x6e
bl 2f // wait_and_transmit

// z
ldr r4, =0x40000000
and r5, r3, r4
cmp r5, r4
ldreq r9, =0x5a
ldrne r9, =0x7a
bl 2f // wait_and_transmit

// c
ldr r4, =0x20000000
and r5, r3, r4
cmp r5, r4
ldreq r9, =0x43
ldrne r9, =0x63
bl 2f // wait_and_transmit

// v
ldr r4, =0x10000000
and r5, r3, r4
cmp r5, r4
ldreq r9, =0x56
ldrne r9, =0x76
bl 2f // wait_and_transmit

//,
ldr r4, =12f // comma
bl 1f // print_string

// i
ldr r4, =0x80
and r5, r3, r4
cmp r5, r4
ldreq r9, =0x49
ldrne r9, =0x69
bl 2f // wait_and_transmit

// f
ldr r4, =0x40
and r5, r3, r4
cmp r5, r4
ldreq r9, =0x46
ldrne r9, =0x66
bl 2f // wait_and_transmit

//", "
ldr r4, =12f // comma
bl 1f // print_string

// arm thumb jazelle
ldr r2, =0x01000020
and r5, r3, r2

cmp r5, #0
ldreq r4, =15f // arm_mode

cmp r5, #0x20
ldreq r4, =16f // thumb_mode

cmp r5, #0x01000000
ldreq r4, =17f // jazelle_mode

cmp r5, r2
ldreq r4, =18f // thumbee_mode

bl 1f // print_string

// processor mode
and r5, r3, #0b11111

cmp r5, #0b10000
ldreq r4, =19f // user_mode

cmp r5, #0b10001
ldreq r4, =20f // fiq_mode

cmp r5, #0b10010
ldreq r4, =21f // irq_mode

cmp r5, #0b10011
ldreq r4, =22f // svc_mode

cmp r5, #0b10110
ldreq r4, =23f // monitor_mode

cmp r5, #0b10111
ldreq r4, =24f // abort_mode

cmp r5, #0b11010
ldreq r4, =25f // hyp_mode

cmp r5, #0b11011
ldreq r4, =26f // undefined_mode

cmp r5, #0b11111
ldreq r4, =27f // system_mode

bl 1f // print_string

// cpsr raw �� ���
ldr r5, =0xF0000000			// �� ��° �ڸ��� ������� ����
mov r6, #28					// ���� �� ��� ���������� ����Ʈ �ؾ��ϴ��� ����
mov r4, r3

bl 5f // extract_digit

// ")"
ldr r9, =0x29
bl 2f // wait_and_transmit

// \r\n
ldr r4, =11f // new_line
bl 1f // print_string
// cpsr ��� ��

ldr r4, =13f // seperator
bl 1f // print_string

// cpsr ����
msr cpsr_all, r3

// r0-r12, lr ����
ldr r14, =10f // backup_registers
ldmia r14!, {r0-r12}
ldr r14, [r14]

b 6f // done

// r4�� ����� �޸� �ּҿ� �ִ� string ���
1: // print_string
	push {lr}
	ldrb r9, [r4], #1

	bl 2f // wait_and_transmit

	cmp r9, #0
	bne 1b // print_string

	pop {lr}
	moveq pc, lr

// tx ���ۿ� ������ ���� ������ ��� �� ����
2: // wait_and_transmit
	ldr r10, [r11]
	and r10, r10, #0x10
	cmp r10, #0x10
	beq 2b // wait_and_transmit

	strb r9, [r12]

	mov pc, lr

3: // loop_register
	ldmfd sp!, {r4} 			// ���ÿ� �����ص� �������� �ε�
	str r4, [r5], #4			// �������� ������ ���� �� ����
	push {r5}					// �������� ������ ���� �����ص� �޸� ��ġ push

	// r0 - r12 ���
	push {lr}
	bl 4f // print_register
	pop {lr}

	add r7, r7, #1

	cmp r7, #13
	popne {r5}					// �������� ������ ���� �����ص� �޸� ��ġ pop
	bne 3b // loop_register

	// r13 ���
	push {lr}
	mov r4, r0
	bl 4f // print_register
	add r7, r7, #1
	pop {lr}

	// r14 ���
	// lr ���
	pop {r5}
	str r1, [r5]

	push {lr}
	mov r4, r1
	bl 4f // print_register
	add r7, r7, #1
	pop {lr}

	// r15 ���
	push {lr}
	mov r4, r2
	bl 4f // print_register
	add r7, r7, #1
	pop {lr}

	mov pc, lr

4: // print_register
	push {lr}
	// "r{num} = 0x" ���
	// r
	ldr r9, =0x72
	bl 2b // wait_and_transmit

	// �������� ��ȣ�� 10���� ũ�� 10�� �ڸ����� 1���� ���
	mov r8, r7

	cmp r7, #10
	ldrge r9, =0x31
	subge r8, r7, #10
	blge 2b // wait_and_transmit

	// �������� ��ȣ�� 1���ڸ��� ���
	add r9, r8, #48
	bl 2b // wait_and_transmit

	// �����̽�
	ldr r9, =0x20
	bl 2b // wait_and_transmit

	// �������� ��ȣ�� ���ڸ����� �����̽� �� �� ���
	cmp r7, #10
	bllt 2b // wait_and_transmit

	// =
	ldr r9, =0x3d
	bl 2b // wait_and_transmit

	// �����̽�
	ldr r9, =0x20
	bl 2b // wait_and_transmit

	// 0
	ldr r9, =0x30
	bl 2b // wait_and_transmit

	// x
	ldr r9, =0x78
	bl 2b // wait_and_transmit

	ldr r5, =0xF0000000			// �� ��° �ڸ��� ������� ����
	mov r6, #28					// ���� �� ��� ���������� ����Ʈ �ؾ��ϴ��� ����

	bl 5f // extract_digit

	cmp r7, #3				// �������� 4�� ������ �� �ٲ�
	cmpne r7, #7
	cmpne r7, #11
	cmpne r7, #15

	ldreq r4, =11f // new_line	// 4���� ��������� �ٹٲ�
	ldrne r4, =12f // comma		// �׷��� ������ ", "
	bl 1b // print_string

	pop {lr}
	mov pc, lr


5: // extract_digit
	and r9, r4, r5			// �������� ������ Ư�� �ڸ� �� ����
	lsr r9, r9, r6			// ���� ������ 4��Ʈ�� ����Ʈ

	cmp r9, #10
	addlt r9, #48
	addge r9, #87

	push {lr}
	bl 2b // wait_and_transmit	// ������ ���� ���
	pop {lr}

	lsr r5, r5, #4			// ���� �ڸ� �� �̵�
	sub r6, r6, #4			// ����Ʈ Ƚ�� ����

	cmp r6, #12				// 4�ڸ� �� ��� ������ _ ���
	moveq r9, #95
	pusheq {lr}
	bleq 2b // wait_and_transmit
	popeq {lr}

	cmp r6, #-4				// ��� �ڸ� ���� ���� �� ������ �ݺ�
	bne 5b // extract_digit

	mov pc, lr

	.ltorg

6: // done
	nop

.data
.align 4
10: // backup_registers
	.space 56, 0x0

11: // new_line
	.byte 0x0d
	.byte 0x0a
	.byte 0x00

12: // comma
	.ascii ", "
	.byte 0x00

13: // seperator
	.ascii "--------------------------------------------------------------------------"
	.byte 0x0D
	.byte 0x0A
	.byte 0x00

14: // cpsr
	.ascii "cpsr = "
	.byte 0x00

15: // arm_mode
	.ascii "ARM mode, current mode = "
	.byte 0x00

16: // thumb_mode
	.ascii "Thumb mode, current mode = "
	.byte 0x00

17: // jazelle_mode
	.ascii "Jazelle mode, current mode = "
	.byte 0x00

18: // thumbee_mode
	.ascii "ThumbEE mode, current mode = "
	.byte 0x00

19: // user_mode
	.ascii "USR ( =0x"
	.byte 0x00

20: // fiq_mode
	.ascii "FIQ ( =0x"
	.byte 0x00

21: // irq_mode
	.ascii "IRQ ( =0x"
	.byte 0x00

22: // svc_mode
	.ascii "SVC ( =0x"
	.byte 0x00

23: // monitor_mode
	.ascii "MON ( =0x"
	.byte 0x00

24: // abort_mode
	.ascii "ABT ( =0x"
	.byte 0x00

25: // hyp_mode
	.ascii "HYP ( =0x"
	.byte 0x00

26: // undefined_mode
	.ascii "UND ( =0x"
	.byte 0x00

27: // system_mode
	.ascii "SYS ( =0x"
	.byte 0x00

.text
.endm

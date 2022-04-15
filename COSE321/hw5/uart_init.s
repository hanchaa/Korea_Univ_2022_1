#include "uart_regs.h"

.macro UART_init

/*
# 1.Reset controller
	ldr		r0, =slcr_UART_RST_CTRL
	ldr		r1, [r0, #0]			// read slcr.UART_RST_CTRL
	orr		r1, r1, #0x0000000A		// set both slcr.UART_RST_CTRL[UART1_REF_RST, UART1_CPU1X_RST] (bit[3][1])
	str		r1, [r0, #0]			// update

# 2.Configure I/O signal routing
# 3.Configure UART_Ref_Clk
	ldr		r0, =slcr_UART_CLK_CTRL
	ldr		r1, =0x00001402			// write 0x00001402 to slcr.UART_CLK_CTRL @ mov	r1, #0x00001402	(*** ERROR ***)
	str		r1, [r0, #0]			// update
*/

# 4.Configure UART


# 	4-1. Configure UART packet format
	ldr		r0, =uart_Mode_reg0
	mov		r1, #0x00000020			// Normal mode, 1 stop bit, 8-bit, bit[5:3] = No Parity (1xx),
	str		r1, [r0]

# 	4-2. Configure the Baud Rate to 115,200bps and Enable UART
	# a-b. Disable RX  and TX
	ldr		r0, =uart_Control_reg0
	ldr		r1, [r0]				// read uart.Control_reg0
	bic		r1, r1, #0x0000003C		// clear TXDIS, TXEN, RXDIS, RXEN (bit[5][4][3][2])
	orr		r1, r1, #0x00000028		// TXDIS = 1 TXEN = 0 RXDIS = 1 RXEN = 0 (bit[5][4][3][2])
	str		r1, [r0]				// write

	# c-d. Write the calculated CD value and BDIV for 115,200bps
	# 115,740 bps =  50MHz / (27*(15+1))
	ldr		r0, =uart_Baud_rate_gen_reg0
	mov		r1, #0x0000001B			// CD = 27
	str		r1, [r0]				// Write to uart.Baud_rate_gen_reg0
	ldr		r0, =uart_Baud_rate_divider_reg0
	mov		r1, #0x0000000F			// BDIV = 15
	str		r1, [r0]				// Write to uart.Baud_rate_divider_reg0

	# e. Reset TX and RX
	ldr		r0, =uart_Control_reg0
	ldr		r1, [r0]				// Read uart.Control_reg0
	orr		r1, r1, #0x00000003		// set TXRST, RXRST (bit[1][0]:self-clearing) - this resets Tx and Rx
	str		r1, [r0]				// Write

	# f-g. Enable RX  and TX
	ldr		r0, =uart_Control_reg0
	ldr		r1, [r0]				// read uart.Control_reg0
	bic		r1, r1, #0x0000003C		// clear TXDIS, TXEN, RXDIS, RXEN (bit[5][4][3][2])
	orr		r1, r1, #0x00000014		// TXDIS = 0 TXEN = 1 RXDIS = 0 RXEN = 1 (bit[5][4][3][2])
	str		r1, [r0]				// Write

# 	4-5. Enable the Controller
	ldr		r0, =uart_Control_reg0
	ldr		r1, =0x00000117			// 0x00000117 to uart.Control_reg0
	str		r1, [r0]				// Write

.endm

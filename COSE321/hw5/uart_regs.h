#define slcr_base			0xF8000000
#define slcr_UART_CLK_CTRL	slcr_base + 0x154
#define slcr_UART_RST_CTRL	slcr_base + 0x228

#define uart_base						0xE0001000
#define uart_Control_reg0				uart_base
#define uart_Mode_reg0					uart_base + 0x00000004
#define uart_Baud_rate_gen_reg0			uart_base + 0x00000018
#define uart_Baud_rate_divider_reg0		uart_base + 0x00000034
#define uart_Modem_ctrl_reg0			uart_base + 0x00000024
#define uart_Modem_sts_reg0 			uart_base + 0x00000028
#define uart_TX_RX_FIFO0				uart_base + 0x00000030
#define uart_Channel_sts_reg0			uart_base + 0x0000002C

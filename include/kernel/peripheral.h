/**
 * @file peripheral.h
 * @brief Definition-only file containing useful memory addresses for various
 * peripherals
 */

#ifndef _PERIPHERAL_H
#define _PERIPHERAL_H

#define SDRAM_START 0x00000000

#ifdef RPIBPLUS
#define PERIPHERAL_BASE 0x20000000
#else
#define PERIPHERAL_BASE 0x3f000000
#endif

#define PERIPHERAL_LEN	0x01000000
#define PERIPHERAL_END	(PERIPHERAL_BASE + PERIPHERAL_LEN)

#define SYSTIMER_OFFSET		0x3000
#define INTERRUPTS_OFFSET	0xb000
#define MAILBOX_OFFSET		0xb880
#define GPIO_OFFSET			0x200000
#define UART_OFFSET			0x201000
#define EMMC_OFFSET			0x300000
#define USB_OFFSET			0x980000

#endif

/**
 * @file gpio.h
 * @brief Code for general purpose I/O, including UART and ACT LED interaction
 */

#ifndef _GPIO_H
#define _GPIO_H

#include <stdint.h>
#include <kernel/peripheral.h>

#if defined ( RPI1 )
#define ACT_GPFSEL GPFSEL1
#define ACT_GPFBIT 18
#define ACT_GPSET GPSET0
#define ACT_GPCLR GPCLR0
#define ACT_GPBIT 16

#elif defined( RPIBPLUS ) || defined( RPI2 )
#define ACT_GPFSEL GPFSEL4
#define ACT_GPFBIT 21
#define ACT_GPSET GPSET1
#define ACT_GPCLR GPCLR1
#define ACT_GPBIT 15

#endif

enum {

	/* GPIO register's base address */
	GPIO_BASE = PERIPHERAL_BASE + GPIO_OFFSET,

	/* GPIO function select */
	GPFSEL0 = (GPIO_BASE + 0x00),
	GPFSEL1 = (GPIO_BASE + 0x04),
	GPFSEL2 = (GPIO_BASE + 0x08),
	GPFSEL3 = (GPIO_BASE + 0x0c),
	GPFSEL4 = (GPIO_BASE + 0x10),
	GPFSEL5 = (GPIO_BASE + 0x14),

	GPSET0 = (GPIO_BASE + 0x1c),
	GPSET1 = (GPIO_BASE + 0x20),
	GPCLR0 = (GPIO_BASE + 0x28),
	GPCLR1 = (GPIO_BASE + 0x2c),

	GPPUD = (GPIO_BASE + 0x94),
	GPPUDCLK0 = (GPIO_BASE + 0x98),

	/* UART base address */
	UART0_BASE = PERIPHERAL_BASE + UART_OFFSET,

	UART0_DR     = (UART0_BASE + 0x00),
	UART0_RSRECR = (UART0_BASE + 0x04),
	UART0_FR     = (UART0_BASE + 0x18),
	UART0_ILPR   = (UART0_BASE + 0x20),
	UART0_IBRD   = (UART0_BASE + 0x24),
	UART0_FBRD   = (UART0_BASE + 0x28),
	UART0_LCRH   = (UART0_BASE + 0x2c),
	UART0_CR     = (UART0_BASE + 0x30),
	UART0_IFLS   = (UART0_BASE + 0x34),
	UART0_IMSC   = (UART0_BASE + 0x38),
	UART0_RIS    = (UART0_BASE + 0x3c),
	UART0_MIS    = (UART0_BASE + 0x40),
	UART0_ICR    = (UART0_BASE + 0x44),
	UART0_DMACR  = (UART0_BASE + 0x48),
	UART0_ITCR   = (UART0_BASE + 0x80),
	UART0_ITIP   = (UART0_BASE + 0x84),
	UART0_ITOP   = (UART0_BASE + 0x88),
	UART0_TDR    = (UART0_BASE + 0x8c)
};

void mmio_write(uint32_t reg, uint32_t data);
uint32_t mmio_read(uint32_t reg);

void uart_init(void);
void uart_putc(unsigned char c);
unsigned char uart_getc(void);
void uart_puts(const char* str);

void act_init(void);
void act_on(void);
void act_off(void);
void act_blink(uint32_t);

#endif

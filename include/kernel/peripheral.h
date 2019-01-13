#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#ifdef RPIBPLUS
#define PERIPHERAL_BASE 0x20000000
#else
#define PERIPHERAL_BASE 0x3f000000
#endif

#define PERIPHERAL_LEN  0x01000000

#define SYSTIMER_OFFSET     0x3000
#define INTERRUPTS_OFFSET   0xb000
#define MAILBOX_OFFSET      0xb880
#define GPIO_OFFSET         0x200000
#define UART_OFFSET         0x201000

#endif

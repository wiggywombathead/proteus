#ifndef _USB_TYPES_H
#define _USB_TYPES_H

#include <stdint.h>

/* result of function call */
typedef enum {
    OK               = 0,   // success
    ERR_GEN          = -1,  // undisclosed error
    ERR_ARG          = -2,  // bad input
    ERR_RETRY        = -3,  // temp issue; rerun func
    ERR_DEV          = -4,  // hardware error
    ERR_INCOMPATIBLE = -5,  // driver doesn't support dev
    ERR_COMPILER     = -6,  // compiler gens unusable code
    ERR_MEM          = -7,  // memory exhausted
    ERR_TIMEOUT      = -8,  // max delay reached
    ERR_DC           = -9   // dev d/c during transfer
} devcall;

/* direction of information flow */
typedef enum {
    HOST_TO_DEV = 0,
    USBDIR_OUT  = 0,
    DEV_TO_HOST = 1,
    USBDIR_IN   = 1,
} usbdirection_t;

typedef enum {
    USBSPD_HIGH = 0,
    USBSPD_FULL = 1,
    USBSPD_LOW  = 2,
} usbspeed_t;

typedef enum {
    TRANSFER_CTRL      = 0,
    TRANSFER_ISO       = 1,
    TRANSFER_BULK      = 2,
    TRANSFER_INTERRUPT = 3,
} usbtransfer_t;

typedef enum {
    BITS_8,
    BITS_16,
    BITS_32,
    BITS_64,
} usbpktsize_t;

static inline usbpktsize_t num_to_size(uint32_t n) {
    if (n <= 8) return BITS_8;
    else if (n <= 16) return BITS_16;
    else if (n <= 32) return BITS_32;
    else return BITS_64;
}

static inline uint32_t size_to_num(usbpktsize_t s) {
    switch (s) {
    case BITS_8:
        return 8;
    case BITS_16:
        return 16;
    case BITS_32:
        return 32;
    default:
        return 64;
    }
}

#endif

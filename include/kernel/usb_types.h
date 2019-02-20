#ifndef _USB_TYPES_H
#define _USB_TYPES_H

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
    USBTFER_CTRL = 0,
    USBTFER_ISO = 1,
    USBTFER_BULK = 2,
    USBTFER_INTERRUPT = 3,
} usbtransfer_t;

#endif

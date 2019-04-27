/**
 * @file attrib.h
 * @brief Macros for GCC attributes
 */

#ifndef _ATTRIB_H
#define _ATTRIB_H

#define PACKED      __attribute__((packed))
#define ALIGNED(n)  __attribute__((aligned(n)))
#define OPTIMIZE(n) __attribute__((optimize(n)))

#endif

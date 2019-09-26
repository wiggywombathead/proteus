/**
 * @file shm.h
 * @brief Code to manage shared memory, to facilitate inter-process
 * communication
 */

#ifndef _SHM_H
#define _SHM_H

#include <stddef.h>

struct shm_section {
	void *addr;
	char name[32];
	char *data;
};

void *shm_open(void);
void shm_write(void *, void *, size_t);
void *shm_read(void *);

#endif

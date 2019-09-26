#include <kernel/atag.h>

/**
 * Determine amount of RAM on board.
 * @param atag Address containing list of atags
 */
uint32_t get_total_mem(struct atag *atag) {

	while (atag->tag != ATAG_NONE) {
		if (atag->tag == ATAG_MEM)
			return atag->mem.size;

		/* move to next tag */
		atag = (struct atag *) (((uint32_t *) atag) + atag->size);
	}

	return 0;
}

/**
 * Return high serial number of board.
 * @param atag Address containing list of atags
 */
uint32_t get_serial_hi(struct atag *atag) {

	while (atag->tag != ATAG_NONE) {
		if (atag->tag == ATAG_SERIAL)
			return atag->serialnr.high;

		/* move to next tag */
		atag = (struct atag *)(((uint32_t *) atag) + atag->size);
	}

	return 0;
}

/**
 * Return low serial number of board.
 * @param atag Address containing list of atags
 */
uint32_t get_serial_lo(struct atag *atag) {

	while (atag->tag != ATAG_NONE) {
		if (atag->tag == ATAG_SERIAL)
			return atag->serialnr.low;

		/* move to next tag */
		atag = (struct atag *)(((uint32_t *) atag) + atag->size);
	}

	return 0;
}

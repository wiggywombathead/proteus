.section ".text"

.global try_lock

/*
 * take pointer to lock variable and use atomic ops to acquire lock
 * return 1 if lock acquired, else 0
 */
try_lock:
    mov r1, #0
    /* swap r1 with location [r0], place [r0] in r2 */
    swp r2, r1, [r0]
    mov r0, r2
    blx lr

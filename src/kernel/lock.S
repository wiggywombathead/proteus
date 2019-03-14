.section .text

.global try_lock

/*
 * int try_lock(int *)
 * take pointer to lock variable and use atomic ops to acquire lock
 * return 1 if lock acquired, else 0
 */
try_lock:
    /* check if locked */
    // ldrex r1, [r0]
    // cmp r1, #0x0
    // beq try_lock

    // /* attempt to lock */
    // mov r1, #0
    // strex r2, r1, [r0]
    // mov r0, r2
    // blx lr

    /* swap r1 with location [r0], place [r0] in r2 */
    mov r1, #0
    swp r2, r1, [r0]
    mov r0, r2
    blx lr

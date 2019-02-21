.section .data

/* scan code lookup tables */

/* regular keys */
.align 3
keys_normal:
    .byte 0x0, 0x0, 0x0, 0x0, 'a', 'b', 'c', 'd'
	.byte 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'
	.byte 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'
	.byte 'u', 'v', 'w', 'x', 'y', 'z', '1', '2'
	.byte '3', '4', '5', '6', '7', '8', '9', '0'
	.byte '\n', 0x0, '\b', '\t', ' ', '-', '=', '['
	.byte ']', '\\', '#', ';', '\'', '`', ',', '.'
	.byte '/', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	.byte 0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+'
	.byte '\n', '1', '2', '3', '4', '5', '6', '7'
	.byte '8', '9', '0', '.', '\\', 0x0, 0x0, '='

/* keys shifted */
.align 3
keys_shift:
	.byte 0x0, 0x0, 0x0, 0x0, 'A', 'B', 'C', 'D'
	.byte 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'
	.byte 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'
	.byte 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@'
	.byte '#', '$', '%', '^', '&', '*', '(', ')'
	.byte '\n', 0x0, '\b', '\t', ' ', '_', '+', '{'
	.byte '}', '|', '~', ':', '@', '`', '<', '>'
	.byte '?', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	.byte 0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+'
	.byte '\n', '1', '2', '3', '4', '5', '6', '7'
	.byte '8', '9', '0', '.', '|', 0x0, 0x0, '='
    

/* variable for kbd address we are reading */
.align 2    // 2^2 = 4-byte aligned
kbd_addr:
    .int 0

/* get scan codes down before the current set on kbd */
kbd_down_prev:
    .rept 6
    .hword 0
    .endr

.section .text
.global kbd_update
kbd_update:
    push {r4, r5, lr}

    kbd .req r4 // alias r4 to kbd
    ldr r0, =kbd_addr
    ldr kbd, [r0]

    /* load keyboard address */
    teq kbd, #0
    bne kbd_found$

get_kbd$:
    bl UsbCheckForChange

    /* if no kbd found, must check for new devices */
    bl KeyboardCount

    /* check if new keyboard has been added */
    teq r0, #0
    ldreq r1, =kbd_addr
    streq r0, [r1]
    beq return$

    /* no keyboards, so no address */
    mov r0, #0
    bl KeyboardGetAddress

    /* obtain address of first keyboard */
    ldr r1, =kbd_addr
    str r0, [r1]

    /* store address */
    teq r0, #0
    beq return$
    mov kbd, r0

kbd_found$:
    mov r5, #0

    /* loop through all keys and store in kbd_down_prev */
    save_keys$:
        mov r0, kbd
        mov r1, r5
        bl KeyboardGetKeyDown

        ldr r1, =kbd_down_prev
        add r1, r5, lsl #1
        strh r0, [r1]
        add r5, #1
        cmp r5, #6
        blt save_keys$

    /* get the new keys */
    mov r0, kbd
    bl KeyboardPoll

    /* check it worked */
    teq r0, #0
    bne get_kbd$

return$:
    pop {r4, r5, pc}
    .unreq kbd  // remove the alias

/*
 * key_was_down()
 * returns r0 = 0 if given scan code not in kbd_down_prev values,
 * else non-zero
 */
.global key_was_down
key_was_down:
    ldr r1, =kbd_down_prev
    mov r2, #0

    /* search for key */
    search_key$:
        ldrh r3, [r1]
        teq r3, r0
        moveq r0, #1
        moveq pc, lr

        add r1, #2
        add r2, #1
        cmp r2, #6
        blt search_key$

    mov r0, #0
    mov pc, lr

.global kbd_getchar
kbd_getchar:
    /* check we have a keyboard */
    ldr r0, =kbd_addr
    ldr r1, [r0]
    teq r1, #0
    moveq r0, #0
    moveq pc, lr

    push {r4, r5, r6, lr}

    // alias kbd and key
    kbd .req r4
    key .req r6

    mov r4, r1
    mov r5, #0

    /* r5 holds key index, r4 holds keyboard address */
    key_loop$:
        mov r0, kbd
        mov r1, r5
        bl KeyboardGetKeyDown
        
        /* if 0, either error or no more keys */
        teq r0, #0
        beq loop_break$

        /* ignore if key already down, only want presses */
        mov key, r0
        bl key_was_down
        teq r0, #0
        bne loop_continue$

        /* scancode outside of lookup table */
        cmp key, #104
        bge loop_continue$

        /* get mod keys to deduce character */
        mov r0, kbd
        bl KeyboardGetModifiers

        /* LShift = 0010, RShift = 0010, both = 00100010 */
        tst r0, #0x22           // 0 iff LShift = RShift = 0
        ldreq r0, =keys_normal
        ldrne r0, =keys_shift
        
        /* load key from lookup table */
        ldrb r0, [r0, key]

        teq r0, #0
        bne kbd_getchar_return$

    loop_continue$:
        add r5, #1
        cmp r5, #6
        blt key_loop$

    loop_break$:
        mov r0, #0

kbd_getchar_return$:
    pop {r4, r5, r6, pc}
    .unreq kbd
    .unreq key

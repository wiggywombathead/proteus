#include <kernel/keyboard.h>

/*
uint8_t kbd_ctrl_read_status(void) {
    return (KBD_CTRL_STATUS_REG);
}

void kbd_ctrl_send_cmd(uint8_t cmd) {
    while (1)
        if (kbd_ctrl_read_status() & KBD_CTRL_MASK_INPUT_BUF == 0)
            break;
    
    (KBD_CTRL_CMD_REG, cmd);
}

uint8_t kbd_enc_read_buf(void) {
    return (KBD_ENC_INPUT_BUF);
}

void kbd_enc_send_cmd(uint8_t cmd) {
    while (1)
        if (kbd_ctrl_read_status() & KBD_CTRL_MASK_INPUT_BUF == 0)
            break;

    (KBD_ENC_CMD_REG, cmd);
}

void kbd_set_leds(bool scroll, bool num, bool caps) {
    uint8_t data = 0;

    data |= (scroll);
    data |= (num << 1);
    data |= (caps << 2);

    kbd_enc_send_cmd(KBD_ENC_SET_LED);
    kbd_enc_send_cmd(data);
}

void kbd_set_led(uint8_t val) {
    kbd_enc_send_cmd(KBD_ENC_SET_LED);
    kbd_enc_send_cmd(val);
}

bool kbd_self_test(void) {
    kbd_ctrl_send_cmd(KBD_CTRL_SELF_TEST);

    while (1)
        if (kbd_ctrl_read_status() & KBD_CTRL_MASK_OUTPUT_BUF)
            break;

    return (kbd_enc_read_buf() == 0x55) ? true : false;
}

void kbd_disable(void) {
    kbd_ctrl_send_cmd(KBD_CTRL_KBD_DISABLE);
    kbd_disabled = true;
}

void kbd_reset_system(void) {
    kbd_ctrl_send_cmd(KBD_CTRL_WRITE_OUT);
    kbd_enc_send_cmd(KBD_CTRL_SYS_RESET);
}

void kbd_init(void) {
    register_irq_handler(
}

static void kbd_irq_handler(void) {

}

static void kbd_irq_clearer(void) {

}
*/

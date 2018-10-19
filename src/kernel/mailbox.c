#include <kernel/mailbox.h>
#include <kernel/memory.h>

struct mail_message mailbox_read(int channel) {
    struct mail_status status;
    struct mail_message result;

    // ensure message is from correct channel
    do {

        // make sure there is mail to receive
        do {
            status = *MAIL0_STATUS;
        } while (status.empty);

        // actually get the message
        result = *MAIL0_READ;

    } while (result.channel != channel);

    return result;
}

void mailbox_send(struct mail_message msg, int channel) {
    struct mail_status status;
    msg.channel = channel;

    // make sure there is room for the message
    do {
        status = *MAIL0_STATUS;
    } while (status.full);

    // actually write the message
    *MAIL0_WRITE = msg;
}

static uint32_t get_value_buf_len(struct property_msg_tag *tag) {

    switch (tag->prop_tag) {
    case FB_ALLOCATE_BUFFER:
    case FB_GET_PHYSICAL_DIMENSIONS:
    case FB_SET_PHYSICAL_DIMENSIONS:
    case FB_GET_VIRTUAL_DIMENSIONS:
    case FB_SET_VIRTUAL_DIMENSIONS:
        return 8;
    case FB_GET_BITS_PER_PIXEL:
    case FB_SET_BITS_PER_PIXEL:
    case FB_GET_BYTES_PER_ROW:
        return 4;
    case FB_RELEASE_BUFFER:
    default:
        return 0;
    }

}

int send_messages(struct property_msg_tag *tags) {

    struct property_msg_buf *msg;
    struct mail_message mail;
    
    uint32_t bufsize, len, bufpos, i;
    bufsize = 0;

    for (i = 0; tags[i].prop_tag != NULL_TAG; i++)
        bufsize += get_value_buf_len(&tags[i]) + 3*sizeof(uint32_t);

    bufsize += 3 * sizeof(uint32_t);

    if (bufsize % 16 != 0)
        bufsize += 16 - (bufsize % 16);

    msg = kmalloc(bufsize);

    if (!msg)
        return -1;

    msg->size = bufsize;
    msg->req_res_code = REQUEST;

    for (i = 0, bufpos = 0; tags[i].prop_tag != NULL_TAG; i++) {
        len = get_value_buf_len(&tags[i]);
        msg->tags[bufpos++] = tags[i].prop_tag;
        msg->tags[bufpos++] = len;
        msg->tags[bufpos++] = 0;
        memcpy(msg->tags + bufpos, &tags[i].value_buffer, len);
        bufpos += (len / 4);
    }

    msg->tags[bufpos] = 0;

    mail.data = ((uint32_t) msg) >> 4;

    mailbox_send(mail, PROPERTY_CHANNEL);
    mail = mailbox_read(PROPERTY_CHANNEL);

    if (msg->req_res_code == RESPONSE_ERROR) {
        kfree(msg);
        return 1;
    }

    for (i = 0, bufpos = 0; tags[i].prop_tag != NULL_TAG; i++) {
        len = get_value_buf_len(&tags[i]);
        bufpos += 3;
        memcpy(&tags[i].value_buffer, msg->tags + bufpos, len);
        bufpos += (len / 4);
    }

    kfree(msg);
    return 0;
}

#pragma once

#include <stdint.h>

enum control_msg_type {
    /* general types */
    CTL_MSG_RESP_ERROR = 0,
    CTL_MSG_RESP_SUCCESS,
    /* requests receiving general types as reqponse */
    CTL_MSG_REQ_STOP,
    /* ping-pong */
    CTL_MSG_REQ_PING,
};

struct control_msg_header {
    enum control_msg_type type;
} __attribute__((packed));

struct control_msg_response {
    struct control_msg_header hdr;
    uint32_t msglen;
    char msg[];
} __attribute__((packed));

struct control_msg_request_stop {
    struct control_msg_header hdr;
} __attribute__((packed));

struct control_msg_request_ping {
    struct control_msg_header hdr;
    char msg[5];
} __attribute__((packed));

typedef void (*control_handler)(int, enum control_msg_type, uint8_t *,
                                uint32_t);

int control_bind(const char *path);
int control_accept(int fd);
int control_connect(const char *path);
void control_close(int fd);
int control_send_msg(int fd, const void *msg, uint32_t msglen);
int control_recv_msg(int fd, void *msg, uint32_t msglen);
int control_handle_msg(const void *msg, uint32_t msglen,
                       control_handler handler);
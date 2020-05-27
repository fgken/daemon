#pragma once

#include <stdint.h>

int control_bind(const char *path);
int control_accept(int fd);
int control_connect(const char *path);
void control_close(int fd);
int control_send_msg(int fd, const char *msg);
int control_recv_msg(int fd, char *buf, uint32_t buflen);
int control_handle_msg(int fd, const char *msg);
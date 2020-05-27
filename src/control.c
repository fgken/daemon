#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "control.h"
#include "log.h"

static int
set_socket_timeout(int fd)
{
    struct timeval tv = {
        .tv_sec = 15,
        .tv_usec = 0,
    };

    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
        log_err("Failed to set SO_SNDTIMEO to socket: %s", strerror(errno));
        return -1;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
        log_err("Failed to set SO_RCVTIMEO to socket: %s", strerror(errno));
        return -1;
    }

    return 0;
}

int
control_bind(const char *path)
{
    int fd;
    size_t n;
    struct sockaddr_un sa;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        log_err("Failed to create socket: %s", strerror(errno));
        return -1;
    }

    memset(&sa, 0x00, sizeof(sa));
    sa.sun_family = AF_UNIX;
    n = strlcpy(sa.sun_path, path, sizeof(sa.sun_path));
    if (n < strlen(path)) {
        log_err("Failed to bind socket: too long path (%s)", path);
        return -1;
    }
    if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
        log_err("Failed to bind socket: %s", strerror(errno));
        return -1;
    }
    if (listen(fd, 4) == -1) {
        log_err("Failed to listen socket: %s", strerror(errno));
        return -1;
    }

    return fd;
}

int
control_accept(int fd)
{
    struct sockaddr_un sa;
    socklen_t salen = sizeof(sa);

    return accept(fd, (struct sockaddr *)&sa, &salen);
}

int
control_connect(const char *path)
{
    int fd;
    size_t n;
    struct sockaddr_un sa;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        log_err("Failed to create socket: %s", strerror(errno));
        return -1;
    }

    if (set_socket_timeout(fd) == -1) {
        return -1;
    }

    memset(&sa, 0x00, sizeof(sa));
    sa.sun_family = AF_UNIX;
    n = strlcpy(sa.sun_path, path, sizeof(sa.sun_path));
    if (n < strlen(path)) {
        log_err("Failed to connect socket: too long path (%s)", path);
        return -1;
    }
    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
        log_err("Failed to connect socket: %s", strerror(errno));
        return -1;
    }

    return fd;
}

void
control_close(int fd)
{
    if (fd != -1) {
        close(fd);
    }
}

int
control_recv_msg(int fd, char *buf, uint32_t buflen)
{
    ssize_t n;
    n = recv(fd, buf, buflen - 1, 0);
    if (0 <= n) {
        buf[n] = '\0';
    }
    return n;
}

int
control_send_msg(int fd, const char *msg)
{
    return send(fd, msg, strlen(msg), 0);
}

static bool
cmdcmp(const char *msg, const char *cmd)
{
    size_t cmdlen = strlen(cmd);
    return strncmp(msg, cmd, cmdlen) == 0
           && (msg[cmdlen] == '\0' || msg[cmdlen] == ' ');
}

int
control_handle_msg(int fd, const char *msg)
{
    int ret = 0;

    if (cmdcmp(msg, "ping")) {
        control_send_msg(fd, "pong");
    } else if (cmdcmp(msg, "stop")) {
        control_send_msg(fd, "success");
    } else {
        log_err("");
        ret = -1;
    }

    return ret;
}
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "control.h"
#include "log.h"

/* TODO: socket option, send/recv flags */

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
control_recv_msg(int fd, void *msg, uint32_t msglen)
{
    return recv(fd, msg, msglen, 0);
}

int
control_send_msg(int fd, const void *msg, uint32_t msglen)
{
    return send(fd, msg, msglen, 0);
}

int
control_handle_msg(const void *msg, uint32_t msglen, control_handler handler)
{
    return 0;
}
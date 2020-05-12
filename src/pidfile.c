#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "pidfile.h"

static int
parse_pidfile(const char *pidfile)
{
    int fd;
    char buf[32] = {0};
    ssize_t n;
    pid_t pid;
    char *p;

    if ((fd = open(pidfile, O_RDONLY)) == -1) {
        if (errno != ENOENT) {
            log_warn("Failed to open pidfile (%s): %s", pidfile,
                     strerror(errno));
        }
        return -1;
    }

    n = read(fd, buf, sizeof(buf));
    if (n == -1) {
        log_warn("Failed to read pidfile (%s): %s", pidfile, strerror(errno));
        close(fd);
        return -1;
    }
    close(fd);

    if (n == 0) {
        return -1;
    }

    buf[sizeof(buf) - 1] = 0;
    pid = (pid_t)strtol(buf, &p, 10);

    if (*p && *p != '\n') {
        return -1;
    }

    if (pid <= 0) {
        return -1;
    }

    return pid;
}

bool
is_daemon_already_runnning(const char *pidfile)
{
    pid_t pid;

    pid = parse_pidfile(pidfile);
    if (pid == -1) {
        return false;
    }

    if (kill(pid, 0) == 0 || errno == EPERM) {
        log_err("Another daemon already running (pid: %d)", pid);
        return true;
    }

    log_info("Did not exit gracefully last time (pid: %d)", pid);

    return false;
}

int
create_pidfile(const char *pidfile)
{
    FILE *fp;

    if ((fp = fopen(pidfile, "w")) == NULL) {
        return -1;
    }
    if (fprintf(fp, "%lu", (unsigned long)getpid()) < 0) {
        log_err("Failed to write pidfile (%s): %s", pidfile, strerror(errno));
        fclose(fp);
        return -1;
    }
    fclose(fp);

    return 0;
}

int
remove_pidfile(const char *pidfile)
{
    return unlink(pidfile);
}
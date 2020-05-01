#include <stdio.h>
#include <syslog.h>

#include "log.h"

int
daemon_main(int argc, char *argv[])
{
    log_init(LOG_OUTPUT_STDERR | LOG_OUTPUT_SYSLOG, NULL, LOG_DAEMON);
    log_info("Starting daemon...");
    return 0;
}
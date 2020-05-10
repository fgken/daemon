#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "config.h"
#include "daemon.h"
#include "log.h"
#include "options.h"
#include "pidfile.h"

int
daemon_main(int argc, char *argv[])
{
    struct options opts;
    struct config config;
    const char *conffile = DEFAULT_CONFIG_FILE;
    const char *pidfile = DEFAULT_PID_FILE;

    /* Arguments */
    if (parse_options(&opts, argc, argv) != 0) {
        fprintf(stderr, "\n%s", usage());
        exit(1);
    }

    /* Logging */
    log_init(PROGNAME, LOG_DAEMON);
    if (opts.dont_daemonize) {
        log_set_output(LOG_OUTPUT_STDERR);
    }
    if (0 < opts.verbose) {
        log_set_level(LOG_DEBUG);
    }

    /* Config */
    if (opts.conffile != NULL) {
        conffile = opts.conffile;
    }
    if (parse_config(&config, conffile) != 0) {
        exit(1);
    }

    /* PID file */
    if (opts.pidfile != NULL) {
        pidfile = opts.pidfile;
    }
    if (create_pidfile(pidfile) != 0) {
        exit(1);
    }

    /* Daemonize */

    /* Event loop */

    return 0;
}
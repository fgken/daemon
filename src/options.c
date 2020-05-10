#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "daemon.h"
#include "options.h"

#define OPTS_STRING "c:dp:v"

int
parse_options(struct options *opts, int argc, char *const argv[])
{
    struct options options;
    int ch;

    memset(&options, 0x00, sizeof(options));

    while ((ch = getopt(argc, argv, OPTS_STRING)) != -1) {
        switch (ch) {
        case 'c':
            options.conffile = strdup(optarg);
            break;
        case 'd':
            options.dont_daemonize = true;
            break;
        case 'p':
            options.pidfile = strdup(optarg);
            break;
        case 'v':
            options.verbose++;
            break;
        default:
            return -1;
        }
    }

    if (0 < argc - optind)
        return -1;

    *opts = options;
    return 0;
}

const char *
usage(void)
{
    return "Usage:\n"
           "  " PROGNAME " [options]\n"
           "\n"
           "Options:\n"
           "  -c <path>: Path to the config file (default: " DEFAULT_CONFIG_FILE
           ")\n"
           "  -d: Don't daemonize\n"
           "  -p <path>: Path to the pid file (default: " DEFAULT_PID_FILE ")\n"
           "  -v: Verbose logging\n";
}

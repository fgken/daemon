#pragma once

#include <limits.h>
#include <stdbool.h>

struct options {
    const char *conffile;
    int verbose;
    bool dont_daemonize;
};

int parse_options(struct options *opts, int argc, char *const argv[]);
const char *usage(void);

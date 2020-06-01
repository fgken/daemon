#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "log.h"

#define VALUES_MAX_NUM 15

static bool
to_i(const char *str, int *i)
{
    char *end;
    long n;

    if (*str == '\0') {
        return false;
    }

    n = strtol(str, &end, 10);
    if (*end != '\0' || n < INT_MIN || INT_MAX < n) {
        return false;
    }
    *i = n;
    return true;
}

static int
parse_line(struct config *cfg, char *line)
{
    char *key;
    char *values[VALUES_MAX_NUM + 1];
    int num_values;
    char *lasts = NULL;
    char *token;
    const char *sep = " ";

    key = strtok_r(line, sep, &lasts);
    if (key == NULL || key[0] == '#') {
        /* Blank or comment line */
        return 0;
    }

    num_values = 0;
    while ((token = strtok_r(NULL, sep, &lasts)) != NULL) {
        values[num_values++] = token;
        if (num_values == VALUES_MAX_NUM) {
            values[num_values] = NULL;
            break;
        }
    }

    if (strcmp(key, "verbose") == 0) {
        int n;
        if (num_values != 1) {
            log_err("Parse Error: verbose - invalid number of values (%d)",
                    num_values);
            return -1;
        }
        if (!to_i(values[0], &n)) {
            log_err("Parse Error: verbose - invalid values (%s)", values[0]);
            return -1;
        }
        cfg->verbose = n;
    } else {
        log_err("Parse Error: Invalid line (%s)", line);
        return -1;
    }

    return 0;
}

int
parse_config(struct config *cfg, const char *conffile)
{
    int ret = -1;
    FILE *fp = NULL;
    char buf[1024];

    if (conffile == NULL) {
        log_err("Invalid config file path");
        goto end;
    }

    fp = fopen(conffile, "r");
    if (fp == NULL) {
        log_err("Cannot open config file - %s", strerror(errno));
        goto end;
    }

    memset(cfg, 0x00, sizeof(*cfg));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        char *p;
        if ((p = strchr(buf, '\n')) != NULL) {
            *p = '\0';
        }
        if (parse_line(cfg, buf) != 0) {
            return -1;
        }
    }

    ret = 0;
end:

    if (fp != NULL) {
        fclose(fp);
    }

    return ret;
}

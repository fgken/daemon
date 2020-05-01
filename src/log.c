#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <syslog.h>

#include "log.h"

static bool use_stderr = false;
static bool use_syslog = false;
static int lowest_level = LOG_INFO;

void
log_init(int outputs, const char *syslog_ident, int syslog_facility)
{
    use_stderr = outputs & LOG_OUTPUT_STDERR;
    use_syslog = outputs & LOG_OUTPUT_SYSLOG;

    if (use_syslog) {
        openlog(syslog_ident, LOG_PID | LOG_NDELAY, syslog_facility);
    }
}

void
log_set_level(int level)
{
    lowest_level = level;
}

static void
vlog(int level, const char *fmt, va_list ap)
{
    va_list ap2;

    if (lowest_level < level)
        return;

    va_copy(ap2, ap);
    if (use_stderr) {
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
    }
    if (use_syslog) {
        vsyslog(level, fmt, ap2);
    }
    va_end(ap2);
}

#define _VLOG(level, fmt)         \
    do {                          \
        va_list ap;               \
                                  \
        va_start(ap, (fmt));      \
        vlog((level), (fmt), ap); \
        va_end(ap);               \
    } while (0)

void
log_err(const char *fmt, ...)
{
    _VLOG(LOG_ERR, fmt);
}

void
log_warn(const char *fmt, ...)
{
    _VLOG(LOG_WARNING, fmt);
}

void
log_info(const char *fmt, ...)
{
    _VLOG(LOG_INFO, fmt);
}

void
log_debug(const char *fmt, ...)
{
    _VLOG(LOG_DEBUG, fmt);
}
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <syslog.h>

#include "log.h"

static bool use_stderr = false;
static bool use_syslog = true;
static int lowest_level = LOG_INFO;

#ifdef UNIT_TEST
int test_num_of_outputs_to_stderr = 0;
int test_num_of_outputs_to_syslog = 0;
int test_level_of_last_output;

int
vfprintf(FILE *stream, const char *format, va_list ap)
{
    test_num_of_outputs_to_stderr++;
    return 1;
}

int
vprintf(const char *format, va_list ap)
{
    return 1;
}

void
vsyslog(int priority, const char *message, va_list args)
{
    test_num_of_outputs_to_syslog++;
    test_level_of_last_output = priority;
}
#endif

void
log_init(const char *syslog_ident, int syslog_facility)
{
    if (syslog_ident != NULL) {
        openlog(syslog_ident, LOG_PID | LOG_NDELAY, syslog_facility);
    }
}

void
log_set_level(int level)
{
    lowest_level = level;
}

void
log_set_output(int outputs)
{
    use_stderr = outputs & LOG_OUTPUT_STDERR;
    use_syslog = outputs & LOG_OUTPUT_SYSLOG;
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
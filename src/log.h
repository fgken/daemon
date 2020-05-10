#pragma once

#define LOG_OUTPUT_STDERR (1 << 0)
#define LOG_OUTPUT_SYSLOG (1 << 1)

/* syslog_ident syslogを決して使わないならNULLを入れる */
void log_init(const char *syslog_ident, int syslog_facility);

/* default は LOG_INFO */
void log_set_level(int level);

/* default は syslog output */
void log_set_output(int outputs);

void log_err(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_debug(const char *fmt, ...);
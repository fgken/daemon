#pragma once

#define LOG_OUTPUT_STDERR (1 << 0)
#define LOG_OUTPUT_SYSLOG (1 << 1)

void log_init(int outputs, const char *syslog_ident, int syslog_facility);
void log_set_level(int level);
void log_err(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_debug(const char *fmt, ...);
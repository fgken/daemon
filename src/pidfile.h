#pragma once

#include <stdbool.h>

bool is_daemon_already_runnning(const char *pidfile);
int create_pidfile(const char *pidfile);
int remove_pidfile(const char *pidfile);
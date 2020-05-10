#pragma once

#define PROGNAME "daemon"
#define DEFAULT_CONFIG_FILE "/etc/daemon/daemon.conf"
#define DEFAULT_PID_FILE "/var/run/daemon.pid"

int daemon_main(int argc, char *argv[]);

#pragma once

struct config {
    int verbose;
};

int parse_config(struct config *cfg, const char *conffile);

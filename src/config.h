#pragma once

struct config {
    int hoge;
};

int parse_config(struct config *cfg, const char *conffile);

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "log",
    srcs = ["log.c"],
    hdrs = ["log.h"],
    visibility = ["//test/src:__pkg__"],
)

cc_library(
    name = "options",
    srcs = ["options.c"],
    hdrs = ["options.h", "daemon.h"],
    visibility = ["//test/src:__pkg__"],
)

cc_library(
    name = "config",
    srcs = ["config.c"],
    hdrs = ["config.h"],
    visibility = ["//test/src:__pkg__"],
)

cc_library(
    name = "pidfile",
    srcs = ["pidfile.c"],
    hdrs = ["pidfile.h"],
    visibility = ["//test/src:__pkg__"],
    deps = ["log"],
)

cc_library(
    name = "daemon",
    srcs = ["daemon.c"],
    hdrs = [
        "daemon.h",
        "log.h",
        "config.h",
        "options.h",
        "pidfile.h",
    ],
    deps = [
        ":log",
        ":config",
        ":options",
        ":pidfile",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.c"],
    deps = [":daemon", ":log"],
)
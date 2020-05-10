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
    hdrs = ["options.h", "daemon.h", "config.h"],
    visibility = ["//test/src:__pkg__"],
)

cc_library(
    name = "daemon",
    srcs = ["daemon.c"],
    hdrs = ["daemon.h"],
    deps = [":log"],
)

cc_binary(
    name = "main",
    srcs = ["main.c"],
    deps = [
        ":daemon",
        ":log",
    ],
)
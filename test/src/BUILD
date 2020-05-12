cc_test(
    name = "log-test",
    srcs = ["log-test.cc"],
    copts = ["-Iexternal/gtest/include -Isrc"],
    deps = [
        "@gtest//:main",
        "//src:log",
    ],
)

cc_test(
    name = "options-test",
    srcs = ["options-test.cc"],
    copts = ["-Iexternal/gtest/include -Isrc -Wno-writable-strings"],
    deps = [
        "@gtest//:main",
        "//src:options",
    ],
)

cc_test(
    name = "pidfile-test",
    srcs = ["pidfile-test.cc"],
    copts = ["-Iexternal/gtest/include -Isrc"],
    deps = [
        "@gtest//:main",
        "//src:pidfile",
    ],
)
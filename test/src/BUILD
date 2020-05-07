cc_test(
    name = "log-test",
    srcs = ["log-test.cc"],
    copts = ["-Iexternal/gtest/include", "-Itest/src/mock"],
    deps = [
        "@gtest//:main",
        "//src:log",
        "//test/src/mock:mock-vfprintf",
        "//test/src/mock:mock-fprintf",
        "//test/src/mock:mock-vsyslog",
    ],
)
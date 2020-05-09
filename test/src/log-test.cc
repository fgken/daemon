extern "C" {
#include <syslog.h>

#include "log.h"
}

#include "gtest/gtest.h"

extern int test_output_to_stderr;
extern int test_output_to_syslog;
extern int test_level;

void
init_mock(void)
{
    test_output_to_stderr = 0;
    test_output_to_syslog = 0;
    test_level = -1;
}

TEST(LogTest, OutputToStderr)
{
    init_mock();
    log_init(LOG_OUTPUT_STDERR, NULL, 0);
    log_err("LogTest");
    EXPECT_EQ(1, test_output_to_stderr);
    EXPECT_EQ(0, test_output_to_syslog);
}

TEST(LogTest, OutputToSyslog)
{
    init_mock();
    log_init(LOG_OUTPUT_SYSLOG, "test", LOG_DAEMON);
    log_err("LogTest");
    EXPECT_EQ(0, test_output_to_stderr);
    EXPECT_EQ(1, test_output_to_syslog);
}

TEST(LogTest, OutputToStderrAndSyslog)
{
    init_mock();
    log_init(LOG_OUTPUT_STDERR | LOG_OUTPUT_SYSLOG, "test", LOG_DAEMON);
    log_err("LogTest");
    EXPECT_EQ(1, test_output_to_stderr);
    EXPECT_EQ(1, test_output_to_syslog);
}

TEST(LogTest, SeverityLevel)
{
    init_mock();
    log_init(LOG_OUTPUT_STDERR | LOG_OUTPUT_SYSLOG, NULL, LOG_DAEMON);

    /* DEBUG Level */
    log_set_level(LOG_DEBUG);

    log_err("LogTest");
    EXPECT_EQ(1, test_output_to_stderr);
    EXPECT_EQ(1, test_output_to_syslog);
    EXPECT_EQ(LOG_ERR, test_level);

    log_warn("LogTest");
    EXPECT_EQ(2, test_output_to_stderr);
    EXPECT_EQ(2, test_output_to_syslog);
    EXPECT_EQ(LOG_WARNING, test_level);

    log_info("LogTest");
    EXPECT_EQ(3, test_output_to_stderr);
    EXPECT_EQ(3, test_output_to_syslog);
    EXPECT_EQ(LOG_INFO, test_level);

    log_debug("LogTest");
    EXPECT_EQ(4, test_output_to_stderr);
    EXPECT_EQ(4, test_output_to_syslog);
    EXPECT_EQ(LOG_DEBUG, test_level);

    /* INFO Level */
    log_set_level(LOG_INFO);

    log_err("LogTest");
    log_warn("LogTest");
    log_info("LogTest");
    log_debug("LogTest");
    EXPECT_EQ(7, test_output_to_stderr);
    EXPECT_EQ(7, test_output_to_syslog);
    EXPECT_EQ(LOG_INFO, test_level);

    /* ERROR Level */
    log_set_level(LOG_ERR);

    log_err("LogTest");
    log_warn("LogTest");
    log_info("LogTest");
    log_debug("LogTest");
    EXPECT_EQ(8, test_output_to_stderr);
    EXPECT_EQ(8, test_output_to_syslog);
    EXPECT_EQ(LOG_ERR, test_level);
}
extern "C" {
#include <syslog.h>

#include "log.h"
}

#include "gtest/gtest.h"

extern int test_num_of_outputs_to_stderr;
extern int test_num_of_outputs_to_syslog;
extern int test_level_of_last_output;

#define SYSLOG_IDENT "daemon"
#define SYSLOG_FACILITY LOG_DAEMON

void
init_mock(void)
{
    test_num_of_outputs_to_stderr = 0;
    test_num_of_outputs_to_syslog = 0;
    test_level_of_last_output = -1;
}

TEST(LogTest, OutputToDefaultSyslog)
{
    init_mock();
    log_init(SYSLOG_IDENT, SYSLOG_FACILITY);
    log_err("LogTest");
    EXPECT_EQ(0, test_num_of_outputs_to_stderr);
    EXPECT_EQ(1, test_num_of_outputs_to_syslog);
}

TEST(LogTest, OutputToStderr)
{
    init_mock();
    log_init(SYSLOG_IDENT, SYSLOG_FACILITY);
    log_set_output(LOG_OUTPUT_STDERR);
    log_err("LogTest");
    EXPECT_EQ(1, test_num_of_outputs_to_stderr);
    EXPECT_EQ(0, test_num_of_outputs_to_syslog);
}

TEST(LogTest, OutputToStderrAndSyslog)
{
    init_mock();
    log_init(SYSLOG_IDENT, SYSLOG_FACILITY);
    log_set_output(LOG_OUTPUT_STDERR | LOG_OUTPUT_SYSLOG);
    log_err("LogTest");
    EXPECT_EQ(1, test_num_of_outputs_to_stderr);
    EXPECT_EQ(1, test_num_of_outputs_to_syslog);
}

TEST(LogTest, ChangeOutput)
{
    init_mock();
    log_init(SYSLOG_IDENT, SYSLOG_FACILITY);
    log_set_output(LOG_OUTPUT_STDERR);
    log_err("LogTest");
    EXPECT_EQ(1, test_num_of_outputs_to_stderr);
    EXPECT_EQ(0, test_num_of_outputs_to_syslog);

    log_set_output(LOG_OUTPUT_SYSLOG);
    log_err("LogTest");
    EXPECT_EQ(1, test_num_of_outputs_to_stderr);
    EXPECT_EQ(1, test_num_of_outputs_to_syslog);

    log_set_output(LOG_OUTPUT_STDERR | LOG_OUTPUT_SYSLOG);
    log_err("LogTest");
    EXPECT_EQ(2, test_num_of_outputs_to_stderr);
    EXPECT_EQ(2, test_num_of_outputs_to_syslog);
}

TEST(LogTest, SeverityLevel)
{
    init_mock();
    log_init(SYSLOG_IDENT, SYSLOG_FACILITY);
    log_set_output(LOG_OUTPUT_STDERR | LOG_OUTPUT_SYSLOG);

    /* DEBUG Level */
    log_set_level(LOG_DEBUG);

    log_err("LogTest");
    EXPECT_EQ(1, test_num_of_outputs_to_stderr);
    EXPECT_EQ(1, test_num_of_outputs_to_syslog);
    EXPECT_EQ(LOG_ERR, test_level_of_last_output);

    log_warn("LogTest");
    EXPECT_EQ(2, test_num_of_outputs_to_stderr);
    EXPECT_EQ(2, test_num_of_outputs_to_syslog);
    EXPECT_EQ(LOG_WARNING, test_level_of_last_output);

    log_info("LogTest");
    EXPECT_EQ(3, test_num_of_outputs_to_stderr);
    EXPECT_EQ(3, test_num_of_outputs_to_syslog);
    EXPECT_EQ(LOG_INFO, test_level_of_last_output);

    log_debug("LogTest");
    EXPECT_EQ(4, test_num_of_outputs_to_stderr);
    EXPECT_EQ(4, test_num_of_outputs_to_syslog);
    EXPECT_EQ(LOG_DEBUG, test_level_of_last_output);

    /* INFO Level */
    log_set_level(LOG_INFO);

    log_err("LogTest");
    log_warn("LogTest");
    log_info("LogTest");
    log_debug("LogTest");
    EXPECT_EQ(7, test_num_of_outputs_to_stderr);
    EXPECT_EQ(7, test_num_of_outputs_to_syslog);
    EXPECT_EQ(LOG_INFO, test_level_of_last_output);

    /* ERROR Level */
    log_set_level(LOG_ERR);

    log_err("LogTest");
    log_warn("LogTest");
    log_info("LogTest");
    log_debug("LogTest");
    EXPECT_EQ(8, test_num_of_outputs_to_stderr);
    EXPECT_EQ(8, test_num_of_outputs_to_syslog);
    EXPECT_EQ(LOG_ERR, test_level_of_last_output);
}
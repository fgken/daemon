#include "src/log.h"
#include "gtest/gtest.h"

#include <syslog.h>

#include "mock-fprintf.h"
#include "mock-vfprintf.h"
#include "mock-vsyslog.h"

void
mock_enable(void)
{
    mock_fprintf_enabled = 1;
    mock_vfprintf_enabled = 1;
    mock_vsyslog_enabled = 1;
    mock_fprintf_called = 0;
    mock_vfprintf_called = 0;
    mock_vsyslog_called = 0;
}

void
mock_disable(void)
{
    mock_fprintf_enabled = 0;
    mock_vfprintf_enabled = 0;
    mock_vsyslog_enabled = 0;
}

TEST(HelloTest, GetGreet)
{
    mock_enable();
    printf("mock_fprintf_called: %d\n", mock_fprintf_called);
    printf("mock_vfprintf_called: %d\n", mock_vfprintf_called);
    printf("mock_vsyslog_called: %d\n", mock_vsyslog_called);
    vsyslog(0, NULL, NULL);
    fprintf(NULL, NULL, NULL);
    vfprintf(NULL, NULL, NULL);
    printf("mock_fprintf_called: %d\n", mock_fprintf_called);
    printf("mock_vfprintf_called: %d\n", mock_vfprintf_called);
    printf("mock_vsyslog_called: %d\n", mock_vsyslog_called);
    mock_disable();

    EXPECT_EQ("Hello", "Hello");
}
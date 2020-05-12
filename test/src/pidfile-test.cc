#include <cstdio>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

extern "C" {
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "pidfile.h"
}

#include "gtest/gtest.h"

class PidFileTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        pidfile = "/tmp/_test_daemon.pid";
        pid = getpid();
        std::remove(pidfile);
    }

    void
    TearDown() override
    {
        std::remove(pidfile);
    }

    pid_t pid;
    const char *pidfile;
};

TEST_F(PidFileTest, CreatePidfileSuccess)
{
    char pidstr[32];

    ASSERT_EQ(0, create_pidfile(pidfile));
    std::ifstream t(pidfile);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    snprintf(pidstr, sizeof(pidstr), "%lu", (unsigned long)pid);
    EXPECT_EQ(pidstr, str);

    EXPECT_EQ(0, create_pidfile(pidfile));
}

TEST_F(PidFileTest, CreatePidfileOverwrite)
{
    ASSERT_EQ(0, create_pidfile(pidfile));
    EXPECT_EQ(0, create_pidfile(pidfile));
}

TEST_F(PidFileTest, CreatePidfileFailure)
{
    EXPECT_NE(0, create_pidfile("/path/to/invalid/hoge/fuga.pid"));
}

TEST_F(PidFileTest, PidFileDontExist)
{
    EXPECT_EQ(false, is_daemon_already_runnning(pidfile));
}

TEST_F(PidFileTest, DaemonNotRunningButPidFileExist)
{
    std::ofstream outputfile(pidfile);
    outputfile << "99999999";
    outputfile.close();
    EXPECT_EQ(false, is_daemon_already_runnning(pidfile));
}

TEST_F(PidFileTest, InvalidPidFile)
{
    std::ofstream outputfile(pidfile);
    outputfile << "invalidstring";
    outputfile.close();
    EXPECT_EQ(false, is_daemon_already_runnning(pidfile));
}

TEST_F(PidFileTest, DaemonAlreadyRunning)
{
    ASSERT_EQ(0, create_pidfile(pidfile));
    EXPECT_EQ(true, is_daemon_already_runnning(pidfile));
}

TEST_F(PidFileTest, RemovePidfile)
{
    ASSERT_EQ(0, create_pidfile(pidfile));
    EXPECT_EQ(0, remove_pidfile(pidfile));
    EXPECT_EQ(-1, access(pidfile, F_OK));
}

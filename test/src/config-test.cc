extern "C" {
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "config.h"
}

#include "gtest/gtest.h"

class ConfigTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        conffile = "/tmp/_test_daemon.conf";
        int fd = open(conffile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        close(fd);
    }

    void
    TearDown() override
    {
        unlink(conffile);
    }

    void
    WriteFile(const char *str)
    {
        int fd = open(conffile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        write(fd, str, strlen(str));
        close(fd);
    }

    const char *conffile;
};

TEST_F(ConfigTest, BlankConfig)
{
    struct config cfg;
    int res;

    res = parse_config(&cfg, conffile);
    ASSERT_EQ(0, res);
    EXPECT_EQ(0, cfg.verbose);
}

TEST_F(ConfigTest, ValidConfig)
{
    struct config cfg;
    int res;

    WriteFile("verbose 1");
    res = parse_config(&cfg, conffile);
    ASSERT_EQ(0, res);
    EXPECT_EQ(1, cfg.verbose);
}

TEST_F(ConfigTest, Comments)
{
    struct config cfg;
    int res;

    WriteFile("# This is Comment hogehoge\n"
              "verbose 1\n"
              "# Comment");
    res = parse_config(&cfg, conffile);
    ASSERT_EQ(0, res);
    EXPECT_EQ(1, cfg.verbose);
}

TEST_F(ConfigTest, ExtraSpaces)
{
    struct config cfg;
    int res;

    WriteFile("  verbose   1  \n"
              " #Comment \n");
    res = parse_config(&cfg, conffile);
    ASSERT_EQ(0, res);
    EXPECT_EQ(1, cfg.verbose);
}

TEST_F(ConfigTest, ConfigPathIsNULL)
{
    struct config cfg;
    int res;

    res = parse_config(&cfg, NULL);
    ASSERT_NE(0, res);
}

TEST_F(ConfigTest, FileNotExists)
{
    struct config cfg;
    int res;

    res = parse_config(&cfg, "/hoge/fuga/fooooo");
    ASSERT_NE(0, res);
}

TEST_F(ConfigTest, UnknownConfigKey)
{
    struct config cfg;
    int res;

    WriteFile("hogeratyto  1");
    res = parse_config(&cfg, conffile);
    ASSERT_NE(0, res);
}

TEST_F(ConfigTest, VerboseInvalidValue)
{
    struct config cfg;
    int res;

    WriteFile("verbose a");
    res = parse_config(&cfg, conffile);
    ASSERT_NE(0, res);
}

TEST_F(ConfigTest, VerboseTooManyValue)
{
    struct config cfg;
    int res;

    WriteFile("verbose 1 2 3");
    res = parse_config(&cfg, conffile);
    ASSERT_NE(0, res);
}

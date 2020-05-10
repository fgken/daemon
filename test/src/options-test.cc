extern "C" {
#include <string.h>
#include <unistd.h>

#include "options.h"
}

#include "gtest/gtest.h"

#define COUNTOF(x) (sizeof(x) / sizeof(x[0]))

static int
compare_options(const struct options *a, const struct options *b)
{
#define CMP(a, b, member) (a->member == b->member)
#define CMPSTR(s1, s2) (s1 != NULL && s2 != NULL ? !strcmp(s1, s2) : s1 == s2)

    return !(CMPSTR(a->conffile, b->conffile) && CMP(a, b, verbose)
             && CMP(a, b, dont_daemonize));
}

class ParseOptionsTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        optreset = 1;
        optind = 1;
        memset(&opts, 0x00, sizeof(opts));
    }

    struct options opts;
};

TEST_F(ParseOptionsTest, EmptyArgument)
{
    char *argv[] = {"progname", NULL};
    struct options expect = {0};

    EXPECT_EQ(0, parse_options(&opts, COUNTOF(argv) - 1, argv));
    EXPECT_EQ(0, compare_options(&expect, &opts));
}

TEST_F(ParseOptionsTest, AllArguments)
{
    char *argv[] = {"progname", "-c", "/path/to/daemon.conf", "-v", "-d", NULL};
    struct options expect = {
        .conffile = "/path/to/daemon.conf",
        .verbose = 1,
        .dont_daemonize = 1,
    };

    EXPECT_EQ(0, parse_options(&opts, COUNTOF(argv) - 1, argv));
    EXPECT_EQ(0, compare_options(&expect, &opts));
}

TEST_F(ParseOptionsTest, MoreVerbose)
{
    char *argv[] = {"progname", "-vvvv", "-v", NULL};
    struct options expect = {
        .verbose = 5,
    };

    EXPECT_EQ(0, parse_options(&opts, COUNTOF(argv) - 1, argv));
    EXPECT_EQ(0, compare_options(&expect, &opts));
}

TEST_F(ParseOptionsTest, MissingConfigFilePath)
{
    char *argv[] = {"progname", "-c", NULL};
    struct options expect = {0};

    EXPECT_NE(0, parse_options(&opts, COUNTOF(argv) - 1, argv));
    EXPECT_EQ(0, compare_options(&expect, &opts));
}

TEST_F(ParseOptionsTest, UnknownArgument)
{
    char *argv[] = {"progname", "-v", "-z", NULL};
    struct options expect = {0};

    EXPECT_NE(0, parse_options(&opts, COUNTOF(argv) - 1, argv));
    EXPECT_EQ(0, compare_options(&expect, &opts));
}

TEST_F(ParseOptionsTest, ExtraArgument)
{
    char *argv[] = {"progname", "-v", "this-is-extra", NULL};
    struct options expect = {0};

    EXPECT_NE(0, parse_options(&opts, COUNTOF(argv) - 1, argv));
    EXPECT_EQ(0, compare_options(&expect, &opts));
}

TEST(UsageTest, GetUsage)
{
    EXPECT_NE((char *)NULL, usage());
}
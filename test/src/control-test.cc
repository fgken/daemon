#include <thread>

extern "C" {
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "control.h"
}

#include "gtest/gtest.h"

class UnixDomainTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        path = "/tmp/control-test.sock";
        unlink(path);
    }

    void
    TearDown() override
    {
        unlink(path);
    }

    const char *path;
};

TEST_F(UnixDomainTest, ServerClient)
{
    int listen_fd;
    int acpt_fd;
    int conn_fd;
    int n;
    char buf[32];

    listen_fd = control_bind(path);
    ASSERT_NE(-1, listen_fd);

    conn_fd = control_connect(path);
    ASSERT_NE(-1, conn_fd);

    acpt_fd = control_accept(listen_fd);
    ASSERT_NE(-1, acpt_fd);

    n = control_send_msg(conn_fd, "ping");
    ASSERT_EQ(4, n);

    n = control_recv_msg(acpt_fd, buf, sizeof(buf));
    ASSERT_EQ(4, n);
    ASSERT_STREQ("ping", buf);

    n = control_handle_msg(acpt_fd, buf);
    ASSERT_EQ(0, n);

    n = control_recv_msg(conn_fd, buf, sizeof(buf));
    ASSERT_EQ(4, n);
    ASSERT_STREQ("pong", buf);

    control_close(acpt_fd);
    control_close(conn_fd);
    control_close(listen_fd);
}
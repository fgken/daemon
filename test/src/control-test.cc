#include <thread>

extern "C" {
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "control.h"
}

#include "gtest/gtest.h"

class ControlEchoServer : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        path = "/tmp/testsocket";
        unlink(path);
        pthread_create(&tid, NULL, server, (void *)path);
        usleep(10000);
    }

    void
    TearDown() override
    {
        pthread_join(tid, NULL);
        unlink(path);
    }

    static void *
    server(void *arg)
    {
        const char *path = (const char *)arg;
        int fd = -1;
        int acpt_fd = -1;
        char buf[8];
        ssize_t n;

        fd = control_bind(path);
        if (fd == -1) {
            goto end;
        }

        acpt_fd = control_accept(fd);
        if (acpt_fd == -1) {
            goto end;
        }

        n = control_recv_msg(acpt_fd, buf, sizeof(buf));
        if (n == -1 || n == 0) {
            goto end;
        }

        n = control_send_msg(acpt_fd, buf, n);
        if (n == -1) {
            goto end;
        }

    end:
        if (acpt_fd != -1)
            control_close(acpt_fd);
        if (fd != -1)
            control_close(fd);

        return NULL;
    }

    const char *path;
    pthread_t tid;
};

class ControlEchoClient : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        path = "/tmp/testsocket";
        unlink(path);
        pthread_create(&tid, NULL, client, (void *)path);
    }

    void
    TearDown() override
    {
        pthread_join(tid, NULL);
        unlink(path);
    }

    static void *
    client(void *arg)
    {
        const char *path = (const char *)arg;
        int fd;
        char hello[8] = "HELLO";
        char buf[8] = {0};
        int n;

        usleep(10000);

        fd = control_connect(path);
        if (fd == -1) {
            goto end;
        }
        n = control_send_msg(fd, hello, strlen(hello));
        if (n == -1) {
            goto end;
        }
        control_recv_msg(fd, buf, sizeof(buf));
        if (n == -1) {
            goto end;
        }
        control_close(fd);

    end:
        if (fd != -1)
            control_close(fd);

        return NULL;
    }

    const char *path;
    pthread_t tid;
};

TEST_F(ControlEchoServer, ClientTest)
{
    int fd;
    char hello[8] = "HELLO";
    char buf[8] = {0};
    int n;

    fd = control_connect(path);
    ASSERT_NE(-1, fd);
    n = control_send_msg(fd, hello, strlen(hello));
    ASSERT_EQ(5, n);
    n = control_recv_msg(fd, buf, sizeof(buf));
    ASSERT_EQ(5, n);
    ASSERT_STREQ(hello, buf);
    control_close(fd);
}

TEST_F(ControlEchoClient, ServerTest)
{
    int fd = -1;
    int acpt_fd = -1;
    char buf[32];
    ssize_t n;

    fd = control_bind(path);
    ASSERT_NE(-1, fd);
    acpt_fd = control_accept(fd);
    ASSERT_NE(-1, acpt_fd);
    n = control_recv_msg(acpt_fd, buf, sizeof(buf));
    ASSERT_NE(-1, n);
    ASSERT_NE(0, n);
    n = control_send_msg(acpt_fd, buf, n);
    ASSERT_NE(-1, n);
    control_close(acpt_fd);
    control_close(fd);
}
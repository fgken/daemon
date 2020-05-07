extern "C" {
#include <dlfcn.h>
#include <stdarg.h>
#include <syslog.h>
}

#include "mock-vsyslog.h"

int mock_vsyslog_enabled = 0;
int mock_vsyslog_called = 0;
typedef int (*vsyslog_t)(int, const char *, va_list);
static vsyslog_t org = 0;

namespace {
class PreMain {
  public:
    PreMain()
    {
        org = (vsyslog_t)dlsym(RTLD_NEXT, "vsyslog");
    }
} pre_main;
} // namespace

void
vsyslog(int priority, const char *format, va_list ap)
{
    if (mock_vsyslog_enabled) {
        mock_vsyslog_called = 1;
        return;
    }
    return; // org(priority, format, ap);
}
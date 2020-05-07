extern "C" {
#include <dlfcn.h>
#include <stdio.h>
}

#include "mock-vfprintf.h"

int mock_vfprintf_enabled = 0;
int mock_vfprintf_called = 0;
typedef int (*vfprintf_t)(FILE *restrict, const char *, va_list);
static vfprintf_t org = 0;

namespace {
class PreMain {
  public:
    PreMain()
    {
        org = (vfprintf_t)dlsym(RTLD_NEXT, "vfprintf");
    }
} pre_main;
} // namespace

int
vfprintf(FILE *stream, const char *format, va_list ap)
{
    if (mock_vfprintf_enabled) {
        mock_vfprintf_called = 1;
        return 1;
    }
    return org(stream, format, ap);
}
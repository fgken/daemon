extern "C" {
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
}

#include "mock-fprintf.h"

int mock_fprintf_enabled = 0;
int mock_fprintf_called = 0;

int
fprintf(FILE *stream, const char *format, ...)
{
    if (mock_fprintf_enabled) {
        mock_fprintf_called = 1;
        return 1;
    }

    va_list ap;
    int n;
    va_start(ap, format);
    n = vfprintf(stream, format, ap);
    va_end(ap);
    return n;
}
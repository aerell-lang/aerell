#if defined(_WIN32)
#include <windows.h>
#endif

size_t strln(const char* s)
{
    size_t len = 0;
    while(s[len]) len++;
    return len;
}

void w(const char* buf, size_t len)
{
#if defined(_WIN32)
    DWORD written;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(h, buf, (DWORD)len, &written, NULL);
#else
#error "Only Win supported"
#endif
}

void p(const char* s)
{
    size_t len = strln(s);
    w(s, len);
}
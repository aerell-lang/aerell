#if defined(_WIN32)
#include <windows.h>
#endif

size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len]) len++;
    return len;
}

void write(const char* str, size_t len)
{
#if defined(_WIN32)
    DWORD written;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(h, str, (DWORD)len, &written, NULL);
#else
#error "Only Win supported"
#endif
}

void print(const char* str)
{
    size_t len = strlen(str);
    write(str, len);
    write("\n", 1);
}
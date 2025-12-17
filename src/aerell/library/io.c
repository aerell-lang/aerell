#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(_WIN32)
BOOL WINAPI
DllMainCRTStartup([[maybe_unused]] HINSTANCE hinst, [[maybe_unused]] DWORD reason, [[maybe_unused]] LPVOID reserved)
{
    return TRUE;
}
#else
#error "Only Win supported"
#endif

size_t strlen(const char* s)
{
    size_t len = 0;
    while(s[len]) len++;
    return len;
}

void write(const char* buf, size_t len)
{
#if defined(_WIN32)
    DWORD written;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(h, buf, (DWORD)len, &written, NULL);
#else
#error "Only Win supported"
#endif
}

void print(const char* s)
{
    size_t len = strlen(s);
    write(s, len);
}
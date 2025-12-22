#if defined(_WIN32)
#include <windows.h>
#endif

void exit(int code)
{
#if defined(_WIN32)
    ExitProcess(code);
#else
#error "Only Win supported"
#endif
}
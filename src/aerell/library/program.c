#if defined(_WIN32)
#include <windows.h>
#endif

void ext(int cd)
{
#if defined(_WIN32)
    ExitProcess(cd);
#else
#error "Only Win supported"
#endif
}
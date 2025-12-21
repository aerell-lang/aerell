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
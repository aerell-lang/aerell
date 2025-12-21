#if defined(_WIN32)
#include <windows.h>
#endif

void ext(int cd) { ExitProcess(cd); }
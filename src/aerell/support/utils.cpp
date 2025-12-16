#include "aerell/support/utils.h"

#if defined(_WIN32)
#include <windows.h>
#else
#error "Only Win supported"
#endif

namespace Aerell
{

std::filesystem::path getExeDir()
{
#if defined(_WIN64)
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    return std::filesystem::path(path).parent_path();
#else
#error "Only Win supported"
#endif
}

} // namespace Aerell
#include <cstdlib>
#include <format>
#include <sstream>

#include "aerell/linker/linker.h"
#include <aerell/support/utils.h>

#include "lld/Common/Driver.h"

LLD_HAS_DRIVER(coff)
// LLD_HAS_DRIVER(elf)
LLD_HAS_DRIVER(mingw)
// LLD_HAS_DRIVER(macho)
// LLD_HAS_DRIVER(wasm)

#define LLD_COFF_DRIVER                                                                                                \
    {                                                                                                                  \
        {                                                                                                              \
            lld::WinLink, &lld::coff::link                                                                             \
        }                                                                                                              \
    }

#define LLD_ELF_DRIVER                                                                                                 \
    {                                                                                                                  \
        {                                                                                                              \
            lld::Gnu, &lld::elf::link                                                                                  \
        }                                                                                                              \
    }

#define LLD_MINGW_DRIVER                                                                                               \
    {                                                                                                                  \
        {                                                                                                              \
            lld::MinGW, &lld::mingw::link                                                                              \
        }                                                                                                              \
    }

#define LLD_MACHO_DRIVER                                                                                               \
    {                                                                                                                  \
        {                                                                                                              \
            lld::Darwin, &lld::macho::link                                                                             \
        }                                                                                                              \
    }

#define LLD_WASM_DRIVER                                                                                                \
    {                                                                                                                  \
        {                                                                                                              \
            lld::Wasm, &lld::wasm::link                                                                                \
        }                                                                                                              \
    }

namespace aerell
{

std::vector<std::string> Linker::libPathFlags;

void Linker::initialize()
{
    const char* pathEnv = std::getenv("PATH");
    if(pathEnv == nullptr) return;

#ifdef _WIN32
    char delimiter = ';';
#else
    char delimiter = ':';
#endif

    std::stringstream ss(pathEnv);
    std::string token;

    bool found = false;

    auto exeBinPath = getExeDir().append("../bin").lexically_normal();

    while(std::getline(ss, token, delimiter))
    {
        std::filesystem::path path(token);
        if(std::filesystem::exists(path) && std::filesystem::equivalent(path, exeBinPath))
        {
            found = true;
            break;
        }
    }

    if(!found) ss = std::stringstream{exeBinPath.generic_string() + delimiter + pathEnv};
    else
    {
        ss.clear();
        ss.seekg(0);
    }

    while(std::getline(ss, token, delimiter))
    {
        std::filesystem::path path(token);

        if(path.filename() == "bin")
        {
            std::filesystem::path libPath = path.parent_path() / "lib";
            if(std::filesystem::exists(libPath) && std::filesystem::is_directory(libPath))
                libPathFlags.push_back(std::format("-L{}", libPath.generic_string()));
        }
    }
}

bool Linker::linking(const char* name)
{
    std::vector<const char*> args = {"ld.lld",   "-m",         "i386pep", "-e", "_start", "-static",
                                     "-laerell", "-lkernel32", name,      "-o", name};

    for(auto& libPathFlag : libPathFlags) args.push_back(libPathFlag.c_str());

    lld::Result result = lld::lldMain(args, llvm::outs(), llvm::errs(), LLD_MINGW_DRIVER);

    return result.retCode != 0;
}

} // namespace aerell
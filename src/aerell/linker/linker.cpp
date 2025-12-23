/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

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

namespace Aerell
{

std::vector<std::string> Linker::libPathFlags;
bool Linker::findLibPathFlagsInit = true;

Linker::Linker()
{
    if(!findLibPathFlagsInit) return;

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

    findLibPathFlagsInit = false;
}

bool Linker::linking(std::string_view filePath)
{
    auto exeName = std::filesystem::path(filePath).filename().replace_extension("exe").generic_string();

    auto objName = std::filesystem::path(exeName).filename().replace_extension("o").generic_string();

    std::vector<const char*> args = {"ld.lld",   "-m",         "i386pep",       "-e", "_start",       "-static",
                                     "-laerell", "-lkernel32", objName.c_str(), "-o", exeName.c_str()};

    for(auto& libPathFlag : libPathFlags) args.push_back(libPathFlag.c_str());

    lld::Result result = lld::lldMain(args, llvm::outs(), llvm::errs(), LLD_MINGW_DRIVER);

    std::filesystem::remove(objName);

    return result.retCode == 0;
}

bool Linker::linking(const std::vector<std::string>& filePaths)
{
    if(filePaths.empty()) return false;

    auto exeName = std::filesystem::path(filePaths.back()).filename().replace_extension("exe").generic_string();

    std::vector<std::string> objNames;
    for(auto& filePath : filePaths)
        objNames.push_back(std::filesystem::path(filePath).filename().replace_extension("o").string());

    std::vector<const char*> args = {"ld.lld", "-m", "i386pep", "-e", "_start", "-static", "-laerell", "-lkernel32"};

    for(auto& libPathFlag : libPathFlags) args.push_back(libPathFlag.c_str());

    for(const auto& objName : objNames) args.push_back(objName.c_str());

    args.push_back("-o");
    args.push_back(exeName.c_str());

    lld::Result result = lld::lldMain(args, llvm::outs(), llvm::errs(), LLD_MINGW_DRIVER);

    for(const auto& objName : objNames) std::filesystem::remove(objName);

    return result.retCode == 0;
}

} // namespace Aerell
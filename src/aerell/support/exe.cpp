/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/support/exe.h"

#if defined(_WIN32)
#include <windows.h>
#else
#error "Only Win supported"
#endif

namespace aerell
{

std::filesystem::path Exe::getDir()
{
#if defined(_WIN64)
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    return std::filesystem::weakly_canonical(path).parent_path();
#else
#error "Only Win supported"
#endif
}

} // namespace aerell
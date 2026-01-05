/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_SUPPORT_EXE_H
#define AERELL_SUPPORT_EXE_H

#include <filesystem>

namespace aerell
{

class Exe
{
  public:
    static std::filesystem::path getDir();
};

} // namespace aerell

#endif
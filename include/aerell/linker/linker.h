/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

#include <vector>

namespace Aerell
{

class Linker
{
  public:
    Linker();
    bool linking(std::string_view filePath);
    bool linking(const std::vector<std::string>& filePaths);

  private:
    static std::vector<std::string> libPathFlags;
    static bool findLibPathFlagsInit;
};

} // namespace Aerell
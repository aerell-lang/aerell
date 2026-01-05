/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_SOURCE_SOURCE_MANAGER_H
#define AERELL_SOURCE_SOURCE_MANAGER_H

#include <unordered_set>
#include <filesystem>
#include <memory>
#include <optional>

#include "aerell/support/exe.h"
#include <aerell/support/ostream.h>
#include <aerell/source/source.h>

namespace aerell
{

class SourceManager
{
  public:
    int import(std::string_view path, bool makeSearchPath = false, OStream* errs = nullptr);
    const Source& last();

  private:
    std::vector<std::filesystem::path> searchPaths{Exe::getDir().append("../src")};
    std::unordered_set<std::filesystem::path> files;
    std::vector<std::unique_ptr<Source>> sources;

    std::optional<std::filesystem::path> find(std::string_view path);
};

} // namespace aerell

#endif
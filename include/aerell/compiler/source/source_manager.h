/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SOURCE_SOURCE_MANAGER_H
#define AERELL_COMPILER_SOURCE_SOURCE_MANAGER_H

#include <memory>
#include <optional>

#include <aerell/support/utils.h>
#include <aerell/support/ostream.h>
#include <aerell/compiler/source/source.h>

namespace aerell
{

class SourceManager
{
  public:
    int import(const char* value, OStream& errs, bool makeAsPrefix = false);
    Source* getLastSource();
    bool hasSource();

  private:
    std::vector<std::string> importSourcePrefixs{getExeDir().append("../src").generic_string()};
    std::vector<std::unique_ptr<Source>> sources;
    bool contain(const std::filesystem::path& path);
    std::optional<std::filesystem::path> find(const char* value);
};

} // namespace aerell

#endif
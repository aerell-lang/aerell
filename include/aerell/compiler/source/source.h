/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SOURCE_SOURCE_H
#define AERELL_COMPILER_SOURCE_SOURCE_H

#include <filesystem>

#include <llvm/Support/raw_ostream.h>

namespace aerell
{

class Source
{
  public:
    Source(std::filesystem::path&& path);

    bool read(llvm::raw_ostream& errs);

    const std::string& getContent() const;
    const char* getContentData() const;
    const std::string& getPath() const;

    void printErrorMessage(size_t offset, size_t size, const char* msg);

  private:
    std::filesystem::path path;
    std::string pathStr;
    std::string content;
    std::vector<size_t> lineStarts;
};

} // namespace aerell

#endif
/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_SOURCE_SOURCE_H
#define AERELL_SOURCE_SOURCE_H

#include <filesystem>
#include <vector>

#include <aerell/support/ostream.h>

namespace aerell
{

class Source
{
  public:
    Source(const std::filesystem::path& path);

    bool read(OStream* errs = nullptr);

    const std::filesystem::path& path() const;
    std::string_view content() const;

    void print(size_t offset, size_t size, std::string_view message) const;

  private:
    const std::filesystem::path& _path;
    std::string _content;
    std::vector<size_t> _lineStartOffsets;
};

} // namespace aerell

#endif
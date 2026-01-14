// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>
#include <string_view>

namespace aerell
{

class File
{
  public:
    File(std::string_view path);

    const std::filesystem::path& getPath() const;
    const char* getData() const;

  private:
    std::filesystem::path path;
    std::string content;
};

} // namespace aerell
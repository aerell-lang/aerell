// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

namespace aerell
{

class File
{
  public:
    static std::optional<File> load(std::string_view path);

    const std::filesystem::path& getPath() const;
    const char* getData() const;

  private:
    std::filesystem::path path;
    std::string content;
    std::vector<std::uint32_t> lineStartOffsets;

    void setPath(std::string_view path);
};

} // namespace aerell
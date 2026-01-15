// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include "aerell/token/lexeme.hpp"

namespace aerell
{

class File
{
  public:
    struct Location
    {
        std::uint32_t line = 0;
        std::uint32_t column = 0;
    };

    static std::optional<File> load(std::string_view path);

    const std::filesystem::path& getPath() const;
    const char* getData() const;

    std::uint32_t getLine(std::uint32_t offset) const;
    std::uint32_t getColumn(std::uint32_t line, std::uint32_t offset) const;
    Location toLocation(const Lexeme& lexeme) const;
    std::uint32_t getOffsetFromLine(std::uint32_t line) const;
    std::string_view getLineText(std::uint32_t line) const;
    std::string_view getLexemeText(const Lexeme& lexeme) const;

  private:
    std::filesystem::path path;
    std::string content;
    std::vector<std::uint32_t> lineStartOffsets;

    void setPath(std::string_view path);
};

} // namespace aerell
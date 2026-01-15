// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string_view>
#include <vector>

#include "aerell/file.hpp"
#include "token/lexeme.hpp"
#include "error_code.hpp"

namespace aerell
{

class Message
{
  public:
    static void print(
        const File& file, const std::vector<Lexeme>& lexemes, const std::vector<std::string_view>& messages);
    static void print(const File& file, const Lexeme& lexeme, std::string_view message);

    static void print(const File& file, ErrorCode ec, const std::vector<Lexeme>& lexemes);
    static void print(const File& file, ErrorCode ec, const Lexeme& lexeme);

  private:
};

} // namespace aerell
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
    struct Lexeme
    {
        std::string_view message;
        aerell::Lexeme data;
        bool pointer;
    };

    static void print(const File& file, const std::vector<Lexeme>& lexemes);
    static void print(const File& file, const Lexeme& lexeme);

    static void print(ErrorCode code, const File& file, const std::vector<Lexeme>& lexemes);
    static void print(ErrorCode code, const File& file, const Lexeme& lexeme);

  private:
};

} // namespace aerell
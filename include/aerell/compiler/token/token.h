/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_TOKEN_TOKEN_H
#define AERELL_COMPILER_TOKEN_TOKEN_H

#include <string_view>
#include <vector>

#include <aerell/compiler/source/source.h>
#include "aerell/compiler/token/token_type.h"

namespace aerell
{

class Token
{
  public:
    using Vec = std::vector<Token>;
    using Vecs = std::vector<Vec>;

    TokenType type;
    Source* source = nullptr;
    size_t offset = 0;
    size_t size = 0;

    std::string_view getText() const;

  private:
};

void print(const Token::Vec& vec);

void print(const Token::Vecs& vecs);

} // namespace aerell

#endif
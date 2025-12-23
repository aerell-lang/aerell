/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

#include <aerell/compiler/source/source.h>
#include <string_view>
#include <vector>

#include "aerell/compiler/token/token_type.h"

namespace Aerell
{

class Token
{
  public:
    TokenType type;
    Source* source = nullptr;
    size_t offset = 0;
    size_t size = 0;

    std::string_view getText() const;

  private:
};

void print(const std::vector<Token>& tokens);

} // namespace Aerell
// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/ast/ast.hpp"

namespace aerell
{
class Semantic
{
  public:
    Semantic(AST& ast);
    bool analyze();

  private:
    bool hasError;
    AST& ast;
};
} // namespace aerell
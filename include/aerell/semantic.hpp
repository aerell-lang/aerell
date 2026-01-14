// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "aerell/ast/ast.hpp"

namespace aerell
{
class Semantic
{
  public:
    Semantic(AST& ast);
    bool analyze();

  private:
    AST& ast;

    void analyze(std::uint32_t index);
};
} // namespace aerell
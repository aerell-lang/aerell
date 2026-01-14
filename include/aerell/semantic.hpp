// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/parser.hpp"

namespace aerell
{
class Semantic
{
  public:
    Semantic(Parser& parser);

    AST analyze();

  private:
    Parser& parser;
};
} // namespace aerell
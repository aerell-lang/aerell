// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

#include "aerell/semantic.hpp"
#include "aerell/module.hpp"

namespace aerell
{

class IR
{
  public:
    IR(Semantic& semantic);
    Module gen();

  private:
    AST ast;
    Module module;

    void gen(std::uint32_t index);
};

} // namespace aerell
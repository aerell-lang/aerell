// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ir.hpp"

namespace aerell
{

IR::IR(AST& ast) : ast{ast} {}

Module IR::gen()
{
    this->module.setName(this->ast.getFile().getPath().generic_string());
    return std::move(this->module);
}

} // namespace aerell
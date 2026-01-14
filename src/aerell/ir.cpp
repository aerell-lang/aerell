// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ast/ast.hpp"
#include "aerell/ir.hpp"

namespace aerell
{

IR IR::gen(Semantic& semantic)
{
    AST ast = semantic.analyze();
    ast.debug();
    return {};
}

} // namespace aerell
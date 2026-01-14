// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/semantic.hpp"

namespace aerell
{

Semantic::Semantic(AST& ast) : ast(ast) {}

bool Semantic::analyze()
{
    this->analyze(0);
    return true;
}

void Semantic::analyze(std::uint32_t index)
{
    switch(ast.getKind(index))
    {
    case ASTKind::NONE: break;
    case ASTKind::INTL: break;
    }
}

} // namespace aerell
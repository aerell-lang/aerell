// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ir.hpp"

namespace aerell
{

IR::IR(Semantic& semantic) : ast{semantic.analyze()} {}

Module IR::gen()
{
    this->module.setName(this->ast.getFile().getPath().generic_string());
    this->gen(0);
    return std::move(this->module);
}

void IR::gen(std::uint32_t index)
{
    switch(this->ast.getKind(index))
    {
    case ASTKind::NONE: break;
    case ASTKind::INTL: break;
    }
}

} // namespace aerell
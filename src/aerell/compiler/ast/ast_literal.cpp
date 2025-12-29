/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/ast/ast_literal.h"

namespace Aerell
{

void ASTLiteral::print(llvm::raw_ostream& os, size_t indent) const
{
    os << std::string(indent, ' ') << "Literal\n";
    os << std::string(indent, ' ') << " value: " << this->value->getText() << '\n';
}

} // namespace Aerell
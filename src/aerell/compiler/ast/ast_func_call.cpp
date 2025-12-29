/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <aerell/compiler/ast/ast_func_call.h>

namespace Aerell
{

void ASTFuncCall::print(llvm::raw_ostream& os, size_t indent) const
{
    os << std::string(indent, ' ') << "FuncCall\n";
    os << std::string(indent, ' ') << " name: " << this->ident->getText() << "\n";
    if(this->args.empty()) return;
    os << std::string(indent, ' ') << " args:\n";
    for(const auto& arg : this->args) arg->print(os, indent + 2);
}

} // namespace Aerell
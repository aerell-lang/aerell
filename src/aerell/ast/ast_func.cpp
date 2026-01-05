/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/ast/ast_func.h"

namespace aerell
{

void ASTFunc::print(OStream& os, size_t indent) const
{
    os << std::string(indent, ' ') << "Func\n";
    os << std::string(indent, ' ') << " ident: " << this->ident->lexeme() << "\n";
    if(!this->params.empty())
    {
        os << std::string(indent, ' ') << " params:\n";
        for(auto param : this->params)
        {
            os << std::string(indent + 1, ' ') << " FuncParam\n";
            os << std::string(indent + 2, ' ') << " ident: " << param.ident->lexeme() << "\n";
            os << std::string(indent + 2, ' ') << " type: " << param.type->lexeme() << "\n";
        }
    }
    if(this->ret != nullptr) os << std::string(indent, ' ') << " ret: " << this->ret->lexeme() << "\n";
    if(this->stmts.has_value())
    {
        os << std::string(indent, ' ') << " stmts:\n";
        for(const auto& stmt : this->stmts.value()) stmt->print(os, indent + 2);
    }
}

} // namespace aerell
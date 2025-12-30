/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_H
#define AERELL_COMPILER_IR_IR_H

#include <aerell/compiler/ast/ast_func.h>
#include <aerell/compiler/ast/ast_func_call.h>
#include <aerell/compiler/ast/ast_literal.h>
#include <aerell/compiler/symbol/symbol_func.h>
#include <aerell/compiler/ir/ir_mod.h>
#include <aerell/compiler/ir/ir_val.h>
#include <aerell/compiler/ir/ir_llvm.h>

namespace aerell
{

class IR
{
  public:
    bool generating(const AST::Groups& groups, IRMod::Vec& vec);
    bool generating(const IRMod::Vec& vec, IRllvm::Unit& unit);

  private:
    bool hasError = false;

    IRBlock* block = nullptr;
    IRMod::Ptr mod = nullptr;

    void stmt(const AST::Ptr& ptr);
    IRVal::Ptr expr(const AST::Ptr& ptr);
    IRFunc* funcDecl(const std::string& ident, const SymbolFunc& ctx);
    void func(ASTFunc& ctx);
    IRVal::Ptr funcCall(ASTFuncCall& ctx);
    IRVal::Ptr literal(ASTLiteral& ctx);
};

} // namespace aerell

#endif
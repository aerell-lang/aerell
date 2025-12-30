/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_CODEGEN_CODEGEN_H
#define AERELL_COMPILER_CODEGEN_CODEGEN_H

#include <aerell/compiler/ir/ir.h>

namespace aerell
{

class CodeGen
{
  public:
    static bool obj(const char* name, const IRllvm::Ptr& ptr);
};

} // namespace aerell

#endif
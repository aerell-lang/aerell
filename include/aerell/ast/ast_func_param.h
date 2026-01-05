/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_AST_AST_FUNC_PARAM_H
#define AERELL_AST_AST_FUNC_PARAM_H

#include "aerell/token/token.h"

namespace aerell
{

class ASTFuncParam
{
  public:
    const Token* ident = nullptr;
    const Token* type = nullptr;
};

} // namespace aerell

#endif